#include "gui/view_controller.hpp"
#include "font6mono_data.hpp"
#include "gui/widgets/divider_widget.hpp"
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


class DividerView : public View{
public:
    DividerView(View *parent) : View(parent){
      ui.hdivider.addWidget(&ui.lvdivider);
      ui.hdivider.addWidget(&ui.rvdivider);
      ui.lvdivider.addWidget(&ui.ltext);
      ui.rvdivider.addWidget(&ui.rtext1);
      ui.rvdivider.addWidget(&ui.rtext2);

      ui.ltext.setText("Text1");
      ui.rtext1.setText("Text2");
      ui.rtext2.setText("Text3");
      ui.ltext.setFontSize(2);
      ui.rtext1.setFontSize(2);
      ui.rtext2.setFontSize(2);
      // setWidget(&ui.vdivider);
      setWidget(&ui.hdivider);
    }
protected:
    void handleInput(InputEvent ev){
    }
private:
    struct{
        HDividerWidget hdivider;
        VDividerWidget lvdivider, rvdivider;
        TextWidget ltext, rtext1, rtext2;
    }ui;
};

int main(){

  stdio_init_all();

  while(!stdio_usb_connected());
  sleep_ms(1000);

  MyButton::init_all();
  auto &controller = ViewController::get();
  DividerView dv{nullptr};
  dv.setBackgroundColor(controller.get_default_graphics().create_pen(120, 40, 60));
  controller.set_view(&dv);
  controller.get_default_graphics().set_font(&font6mono);
  controller.get_default_display().set_backlight(100);



  while(true) {
    // printf("[LOOP]\n");
    controller.poll_event_loop();
  }
}