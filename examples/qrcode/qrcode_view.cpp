#include "examples/qrcode_view.hpp"
#include "drivers/button.hpp"
#include "buttons.hpp"

QRCodeView::QRCodeView(gui::View *parent) : View(parent){
    ui.qrcode.setScale(3);
    ui.qrcode.setBorderSize(1);
    ui.qrcode.setData("https://www.youtube.com/watch?v=xvFZjo5PgG0");
    setWidget(&ui.qrcode);
}

void QRCodeView::handleInput(InputEvent ev){
    MyButton *b = (MyButton*)ev.data;
    if(ev.type == ev.ButtonClick)
        switch(b->get_pin()){
            case Buttons::X:{
                ui.qrcode.setMinimumVersion(std::max(1, ui.qrcode.getVersion()-1));
                return;
            }
            case Buttons::Y:{
                ui.qrcode.setMinimumVersion(ui.qrcode.getVersion()+1);
                if((ui.qrcode.getArea()) > 135)
                    ui.qrcode.setMinimumVersion(ui.qrcode.getVersion()-1);
                return;
            }
        }
    View::handleInput(ev);
}