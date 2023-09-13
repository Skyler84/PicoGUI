#include "gui/view_controller.hpp"

#include "pico_display.hpp"
#include "gui/common.hpp"
#include "gui/view.hpp"
#include "drivers/button.hpp"

// #define DEBUG_PRINTF(...) printf("[VIEWCTRL] " __VA_ARGS__)
#define DEBUG_PRINTF(...) 

using namespace gui;

ViewController ViewController::singleton(gui::Display{pimoroni::PicoDisplay::WIDTH, pimoroni::PicoDisplay::HEIGHT, pimoroni::ROTATE_0, false, pimoroni::get_spi_pins(pimoroni::BG_SPI_FRONT)});

ViewController::ViewController(gui::Display display) : 
    display(display), graphics(display.width, display.height, nullptr)
{
    display.set_backlight(100);
    ui.sp.setRequiredSize({0, 16});
    ui.bar.addWidget(&ui.sp);
    ui.div.addWidget(&ui.bar);
    ui.bar.setSeparator(0);
    ui.bar.setSpacing(1);
    ui.div.setSeparator(0);
    ui.div.setSpacing(1);
    ui.txt.setText("hi");
    ui.bar.addWidget(&ui.txt);
    ui.div.setBox({0,0,display.width, display.height});

}

void ViewController::set_view(View *v){
    DEBUG_PRINTF("set_view(%p)\n", v);
    next_view = v;
}

void ViewController::poll_event_loop(){
    // DEBUG_PRINTF("Event loop poll\n");
    if(next_view){
        DEBUG_PRINTF("next view loading\n");
        if(current_view){
            ui.div.removeWidget(current_view);
            current_view->onExit();
            if(auto v = current_view->getParentView(); v && v == next_view){
                v->setChildView(nullptr);
                delete current_view;
            }

        }
        current_view = next_view;
        next_view = nullptr;
        if(current_view){
            ui.div.addWidget(current_view);
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
    // auto *cv = current_view;
    Widget *cv = &ui.div;
    if(cv->needsRelayout() || cv->childNeedsRelayout()){
        DEBUG_PRINTF("relayout-ing\n");
        //keep relayouting until everything is stable
        while(cv->relayout());
        cv->relayoutDone();
    }
    if(cv->needsRedrawing() || cv->childNeedsRedrawing()){
        DEBUG_PRINTF("redraw-ing (%p)\n", cv);
        cv->redraw(graphics, false);
        cv->redrawDone();
        display.update(&graphics);
        DEBUG_PRINTF("redraw-n (%p)\n", cv);
    }
    // DEBUG_PRINTF("Event loop poll completed\n");

}

gui::Color ViewController::getBackgroundColor() const{ 
    return this->current_view?
        this->current_view->m_backgroundColor:
        0;
}