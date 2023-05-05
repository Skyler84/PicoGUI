#include "gui/view_controller.hpp"
#include "font6mono_data.hpp"
#include "gui/widgets/list_widget.hpp"
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


class ListView : public View{
public:
    ListView(View *parent) : View(parent){
        ui.list.addItem(&ui.t1);
        ui.list.addItem(&ui.t2);
        ui.list.addItem(&ui.t3);
        ui.t1.setText("Row item 1");
        ui.t2.setText("Row item 2");
        ui.t3.setText("Row item 3");
        setWidget(&ui.list);
    }
protected:
    void handleInput(InputEvent ev){
    }
private:
    struct{
        ListWidget list;
        TextWidget t1, t2, t3;
    }ui;
};

int main(){

  stdio_init_all();

  while(!stdio_usb_connected());
  sleep_ms(200);

  MyButton::init_all();
  auto &controller = ViewController::get();
  ListView lv{nullptr};
  lv.setBackgroundColor(controller.get_default_graphics().create_pen(120, 40, 60));
  controller.set_view(&lv);
  controller.get_default_graphics().set_font(&font6mono);
  controller.get_default_display().set_backlight(100);



  while(true) {
    // printf("[LOOP]\n");
    controller.poll_event_loop();
  }
}