#include "gui/view_controller.hpp"
#include "font6mono_data.hpp"
#include "gui/widgets/qrcode_widget.hpp"
#include "pico/stdio.h"

#include "drivers/button.hpp"
#include "buttons.hpp"

using namespace gui;

MyButton buttons[4] = {
  MyButton(Buttons::A, MyButton::ACTIVE_LOW, 0),
  MyButton(Buttons::B, MyButton::ACTIVE_LOW, 0),
  MyButton(Buttons::X, MyButton::ACTIVE_LOW, 0),
  MyButton(Buttons::Y, MyButton::ACTIVE_LOW, 0),
};


class QRCodeView : public View{
public:
    QRCodeView(View *parent) : View(parent){
        ui.qrcode.setScale(3);
        ui.qrcode.setBorderSize(2);
        ui.qrcode.setData("https://www.youtube.com/watch?v=xvFZjo5PgG0");
        setWidget(&ui.qrcode);

    }
protected:
    void handleInput(InputEvent ev){
        if(ev.type != ev.ButtonClick)
            return;
        MyButton *b = (MyButton*)ev.data;
        switch(b->get_pin()){
            case Buttons::X:{
                ui.qrcode.setMinimumVersion(std::max(1, ui.qrcode.getVersion()-1));
                break;
            }
            case Buttons::Y:{
                ui.qrcode.setMinimumVersion(ui.qrcode.getVersion()+1);
                if((ui.qrcode.getArea()) > 135)
                    ui.qrcode.setMinimumVersion(ui.qrcode.getVersion()-1);

                break;
            }
        }
    }
private:
    struct{
        QRCodeWidget qrcode;
    }ui;
};

int main(){

  stdio_init_all();

  while(!stdio_usb_connected());
  sleep_ms(1200);

  MyButton::init_all();
  auto &controller = ViewController::get();
  QRCodeView tv{nullptr};
  tv.setBackgroundColor(controller.get_default_graphics().create_pen(120, 40, 60));
  controller.set_view(&tv);
  controller.get_default_graphics().set_font(&font6mono);
  controller.get_default_display().set_backlight(100);



  while(true) {
    // printf("[LOOP]\n");
    controller.poll_event_loop();
  }
}