#include "gui/view_controller.hpp"

#include "pico_display.hpp"
#include "gui/common.hpp"
#include "gui/view.hpp"
#include "drivers/button.hpp"

#define DEBUG_PRINTF(...) printf("[VIEWCTRL] " __VA_ARGS__)
// #define DEBUG_PRINTF(...) 

using namespace gui;

ViewController ViewController::singleton(gui::Display{pimoroni::PicoDisplay::WIDTH, pimoroni::PicoDisplay::HEIGHT, pimoroni::ROTATE_0, false, pimoroni::get_spi_pins(pimoroni::BG_SPI_FRONT)});

ViewController::ViewController(gui::Display display) : 
    display(display), graphics(display.width, display.height, nullptr)
{
    display.set_backlight(100);
}

void ViewController::set_view(View *v){
    next_view = v;
}

void ViewController::poll_event_loop(){
    // DEBUG_PRINTF("Event loop poll\n");
    if(next_view){
        DEBUG_PRINTF("next view loading\n");
        if(current_view)
            current_view->onExit();
        current_view = next_view;
        next_view = nullptr;
        if(current_view){
            current_view->requiresRedraw();
            current_view->onEntry();
        }
        DEBUG_PRINTF("next view loaded\n");
    }

    if(!current_view){
        DEBUG_PRINTF("No view loaded\n");
        return;
    }

    InputEvent::EventType btnEventType[] = {
        [MyButton::ButtonEventType::Released] = InputEvent::ButtonRelease,
        [MyButton::ButtonEventType::Pressed] = InputEvent::ButtonPress,
        [MyButton::ButtonEventType::Clicked] = InputEvent::ButtonClick,
        [MyButton::ButtonEventType::Held] = InputEvent::ButtonHold,
        [MyButton::ButtonEventType::RepeatClicked] = InputEvent::ButtonRepeatClick,
    };

    //handle events
    MyButton::ButtonEvent be;
    while(MyButton::event_queue.try_remove(be)){
        InputEvent e{};
        e.data = be.button;
        if(be.type >= sizeof(btnEventType))
            e.type = InputEvent::NONE;
        else
            e.type = btnEventType[be.type];
        current_view->handleInput(e);
    }

    View::TimerEvent te;
    while(View::timer_events.try_remove(te)){
        DEBUG_PRINTF("test\n");
        if(!te.v)
            continue;
        te.v->onTimer(te.id);
    }

    if(current_view->needsRelayout() || current_view->childNeedsRelayout()){
        DEBUG_PRINTF("relayout-ing\n");
        //keep relayouting until everything is stable
        while(current_view->relayout());
        current_view->relayoutDone();
    }
    if(current_view->needsRedrawing() || current_view->childNeedsRedrawing()){
        DEBUG_PRINTF("redraw-ing (%p)\n", current_view);
        current_view->redraw(graphics, false);
        current_view->redrawDone();
        display.update(&graphics);
        DEBUG_PRINTF("redraw-n (%p)\n", current_view);
    }
    // DEBUG_PRINTF("Event loop poll completed\n");

}

gui::Color ViewController::getBackgroundColor() const{ 
    gui::Color c;
    c = this->current_view?
        this->current_view->m_backgroundColor:
        0;
    return c;
}