#pragma once
namespace gui{
class View;
}

#include "input_event.hpp"
#include "queue.hpp"
#include "gui/widgets/widget.hpp"
#include "gui/view_controller.hpp"
#include "gui/drawable.hpp"
#include <list>

namespace gui
{
        
    class View : protected Widget{
        friend class ViewController;
    public:
        View(View* parent);
        ~View();

        void setBackgroundColor(gui::Color c) { m_backgroundColor = c; }
        gui::Color getBackgroundColor() const { return m_backgroundColor; }
    protected:
        gui::Graphics &get_default_graphics();
        gui::Display &get_default_display();

        int start_timer(long timeout_us);

        View *getParentView() const {
            return static_cast<View*>(getParent()); 
        }
        void setParentView(View *v) {
            this->setParent(v);
        }
        View *getChildView() const {return childView; }
        void setChildView(View* view) { 
            printf("[VIEW] (%p)->setChildView(%p)\n", this, view);
            childView = view; 
            if(childView)
                childView->setParent(this);
        }

        void closeView();

    protected:
        virtual void handleInput(InputEvent ev) = 0;
        virtual void onTimer(int id){};
        virtual void onEntry(){}
        virtual void onExit(){}

        bool close();

        void setWidget(Widget *w);
        Widget *getWidget() { return childWidgets; }

        void redraw(gui::Graphics &g, bool);
        bool relayout();
    private:
        static int64_t timer_handler(alarm_id_t, void *data);


        View *childView;
        Widget *childWidgets = nullptr;
        Color m_backgroundColor;

        enum TimerEventType{
            Timeout,
        };
        struct TimerEvent{
            TimerEventType type;
            alarm_id_t id;
            View *v;
        };
        static Queue<TimerEvent> timer_events;
        std::list<alarm_id_t> m_alarms;
    };

} // namespace gui