#include "gui/view_controller.hpp"
#include "font6mono_data.hpp"
#include "gui/widgets/text_widget.hpp"
#include "buttons.hpp"
#include "drivers/button.hpp"
#include "pico/stdio.h"

using namespace gui;

MyButton buttons[4] = {
  MyButton(Buttons::A, MyButton::ACTIVE_LOW, 0),
  MyButton(Buttons::B, MyButton::ACTIVE_LOW, 0),
  MyButton(Buttons::X, MyButton::ACTIVE_LOW, 0),
  MyButton(Buttons::Y, MyButton::ACTIVE_LOW, 0),
};


class TextView : public View{
public:
    TextView(View *parent) : View(parent){
        ui.text.setFontSize(2);
        ui.text.setText("Press a button!");
        setWidget(&ui.text);
    }
protected:
    void handleInput(InputEvent ev){
        printf("[TV] handleInput()\n");
        if(ev.type != InputEvent::ButtonClick)
            return;
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
        }
    }
private:
    struct{
        TextWidget text;
    }ui;
};

int main(){

  stdio_init_all();

  while(!stdio_usb_connected());
  sleep_ms(200);

  MyButton::init_all();
  auto &controller = ViewController::get();
  TextView tv{nullptr};
  tv.setBackgroundColor(controller.get_default_graphics().create_pen(120, 40, 60));
  controller.set_view(&tv);
  controller.get_default_graphics().set_font(&font6mono);
  controller.get_default_display().set_backlight(100);



  while(true) {
    // printf("[LOOP]\n");
    controller.poll_event_loop();
  }
}