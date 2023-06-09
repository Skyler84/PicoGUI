#pragma once

namespace gui{
class ViewController;
}
#include "gui/view.hpp"
#include "gui/common.hpp"

namespace gui{

    class ViewController final {
        friend class View;

    public:
        static ViewController &get() { return singleton; };

        void set_view(View *view);

        void poll_event_loop();

        gui::Graphics& get_default_graphics() { return graphics;}
        gui::Display& get_default_display() { return display;}

        gui::Color getBackgroundColor() const ;
    private:
        ViewController(gui::Display display);
        static ViewController singleton;
        gui::Display display;
        gui::Graphics graphics;
        View *current_view = nullptr;
        View *next_view = nullptr;
    };
} // namespace gui