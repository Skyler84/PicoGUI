#pragma once

#include "gui/view.hpp"
#include "gui/widgets/divider_widget.hpp"
#include "gui/widgets/text_widget.hpp"

class DividerView : public gui::View{
public:
    DividerView(gui::View *parent);
protected:
    void handleInput(InputEvent ev);
private:
    struct{
        gui::HDividerWidget hdivider;
        gui::VDividerWidget lvdivider, rvdivider;
        gui::TextWidget ltext, rtext1, rtext2;
    }ui;
};