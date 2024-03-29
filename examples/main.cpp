#include "gui/view_controller.hpp"
#include "font6mono_data.hpp"
#include "gui/widgets/qrcode_widget.hpp"
#include "pico/stdio.h"
#include "hardware/watchdog.h"
#include "pico/time.h"
#include "pico/bootrom.h"

#include "drivers/button.hpp"
#include "buttons.hpp"

#include INCLUDE

using namespace gui;

MyButton buttons[4] = {
  MyButton(Buttons::A, Polarity::ACTIVE_LOW, 0),
  MyButton(Buttons::B, Polarity::ACTIVE_LOW, 0),
  MyButton(Buttons::X, Polarity::ACTIVE_LOW, 0),
  MyButton(Buttons::Y, Polarity::ACTIVE_LOW, 0),
};



extern "C" bool tud_connected();

void wait_stdio_usb_connected_timeout(long timeout_ms, long startup_delay_ms){
  if(!tud_connected())
    return;
  absolute_time_t timeout = make_timeout_time_ms(timeout_ms);

  while(!stdio_usb_connected() && absolute_time_diff_us(get_absolute_time(), timeout) > 0){
    watchdog_update();
  }
  timeout = make_timeout_time_ms(startup_delay_ms);
  while(stdio_usb_connected() && absolute_time_diff_us(get_absolute_time(), timeout) > 0){
    watchdog_update();
  } 
}

int main(){

  stdio_init_all();

  if(watchdog_enable_caused_reboot()) {
    reset_usb_boot(0,0);
  }

  watchdog_enable(200, 1);

  wait_stdio_usb_connected_timeout(2500, 500);

  MyButton::init_all();
  auto &controller = ViewController::get();
  CLASSNAME v{nullptr};
  v.setBackgroundColor(controller.get_default_graphics().create_pen(120, 40, 60));
  controller.set_view(&v);
  controller.get_default_graphics().set_font(&font6mono);
  controller.get_default_display().set_backlight(100);

  while(true) {
    controller.poll_event_loop();
    watchdog_update();
  }
}