#pragma once

#include "gui/view.hpp"
#include "gui/widgets/text_widget.hpp"

class TextView : public gui::View{
public:
    TextView(gui::View *parent);
protected:
    void handleInput(InputEvent ev);
private:
    struct{
        gui::TextWidget text;
    }ui;
};