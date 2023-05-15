#include "examples/text_view.hpp"

#include "drivers/button.hpp"
#include "buttons.hpp"

TextView::TextView(gui::View *parent) : View(parent){
    ui.text.setFontSize(2);
    ui.text.setText("Press a button!");
    setWidget(&ui.text);
}

void TextView::handleInput(InputEvent ev){
    printf("[TV] handleInput()\n");
    if(ev.type == InputEvent::ButtonClick){
        MyButton *btn = (MyButton*)ev.data;
        struct{uint pin; const char*name; int scale;} arr[] = {
            {Buttons::A, "A", 1},
            {Buttons::B, "B", 2},
            {Buttons::X, "X", 3},
            {Buttons::Y, "Y", 2},
        };
        for(auto &b : arr){
            if(b.pin != btn->get_pin())
                continue;
            ui.text.setText(std::string(b.name) + " button pressed");
            ui.text.setFontSize(b.scale);
            return;
        }
    }else{
        View::handleInput(ev);
    }
    printf("[TV] handleInput() done\n");
}