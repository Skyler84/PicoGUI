#pragma once

#include "gui/view.hpp"
#include "gui/widgets/qrcode_widget.hpp"


class QRCodeView : public gui::View{
public:
    QRCodeView(gui::View *parent);
protected:
    void handleInput(InputEvent ev);
private:
    struct{
        gui::QRCodeWidget qrcode;
    }ui;
};