#pragma once

#include "gui/view.hpp"
#include "gui/widgets/list_widget.hpp"
#include "gui/widgets/text_widget.hpp"

class ListView : public gui::View{
public:
    ListView(gui::View *parent);
protected:
    void handleInput(InputEvent ev);
private:
    struct{
        gui::ListWidget list;
        gui::TextWidget t1, t2, t3;
    }ui;
};