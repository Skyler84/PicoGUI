#include "gui/view.hpp"

#include "drivers/button.hpp"
#include "buttons.hpp"
#include "pico/time.h"

#define DEBUG_PRINTF(...) printf("[VIEW] " __VA_ARGS__)

using namespace gui;

Queue<View::TimerEvent> View::timer_events{32};

View::View(View *parent) : 
    Widget(parent, {0,0,get_default_display().width, get_default_display().height}), childView(nullptr){
    requiresRedraw();
    if(parent)
        setBackgroundColor(parent->getBackgroundColor());
}

View::~View(){
    //stop all alarms
    while(m_alarms.size()){
        cancel_alarm(m_alarms.front());
        m_alarms.pop_front();
    }
}

Graphics &View::get_default_graphics(){
    return ViewController::get().get_default_graphics();
}
Display &View::get_default_display(){
    return ViewController::get().get_default_display();
}

int View::start_timer(long timeout_us){
    //TODO: keep track of timers
    alarm_id_t id = add_alarm_at(make_timeout_time_us(timeout_us), timer_handler, this, true);
    m_alarms.push_back(id);
    DEBUG_PRINTF("Timer %d set for %dus\n", id, timeout_us);
    return id;
}

int64_t View::timer_handler(alarm_id_t id, void* data){
    DEBUG_PRINTF("Timer %d fired\n", id);
    View *v = (View*)data;
    if(v)
        v->m_alarms.remove(id);
    if(!timer_events.try_add(TimerEvent{Timeout, id, v})){
        DEBUG_PRINTF("Failed to add timer to event queue\n");
    }
    return 0;
}

void View::handleInput(InputEvent ev){
    if(ev.type == InputEvent::ButtonHold && ((MyButton*)ev.data)->get_pin() == Buttons::B)
        close();
}

bool View::close(){
    if(!getParentView())
        return false;
    ViewController::get().set_view(getParentView());
    return true;
}

void View::setWidget(gui::Widget *w) {
    assert(!childWidgets);
    childWidgets = w;
    w->setParent(this);
    DEBUG_PRINTF("setting view widget box\n");
    w->setBox({0,0,get_default_display().width, get_default_display().height});
    requiresRedraw();
    printf("d\n");
}

void View::redraw(gui::Graphics&g, bool){
    if(!childWidgets)
        return;
    if(needsRedrawing()){
        clear(g);
        childWidgets->redraw(g);
        childWidgets->redrawDone();
        return;
    }
    if(!childWidgets->needsRedrawing() && !childWidgets->childNeedsRedrawing())
        return;
    clear(g);
    childWidgets->redraw(g);
    childWidgets->redrawDone();
}

bool View::relayout(){
    DEBUG_PRINTF("relayout()\n");
    if(!childWidgets)
        return false;
    return childWidgets->relayout();
    DEBUG_PRINTF("1\n");
    if(needsRelayout() || childNeedsRelayout()){
        if(childWidgets->relayout())
            return true;
        childWidgets->relayoutDone();
        return false;
    }
    DEBUG_PRINTF("2\n");
    if(!childWidgets->needsRelayout() && !childWidgets->childNeedsRelayout())
        return false;
    if(childWidgets->relayout())
        return true;
    childWidgets->relayoutDone();
    DEBUG_PRINTF("3\n");
    return false;


}