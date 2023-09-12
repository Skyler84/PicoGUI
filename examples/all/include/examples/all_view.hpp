#pragma once

#include "gui/view.hpp"
#include "gui/widgets/divider_widget.hpp"
#include "gui/widgets/text_widget.hpp"
#include "gui/widgets/list_widget.hpp"

#include "examples/text_view.hpp"
#include "examples/divider_view.hpp"
#include "examples/list_view.hpp"
#include "examples/qrcode_view.hpp"

class AllView : public gui::View{
public:
    AllView(gui::View *parent);
protected:
    void handleInput(InputEvent ev);
private:
    struct{
        gui::VDividerWidget div;
        gui::TextWidget header;
        gui::ListWidget list;
        gui::TextWidget li_text, li_dividier, li_list, li_qrcode;
    }ui;
    
    TextView tv;
    DividerView dv;
    ListView lv;
    QRCodeView qv;

    void selectApplication(size_t idx);
};