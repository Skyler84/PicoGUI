#pragma once

#include "drivers/rp2040/sio_pin.hpp"
#include "pico/time.h"
#include "queue.hpp"

#define DEBOUNCE_TIME 15 /*ms*/

class MyButton;

namespace gui{
  class ViewController;
}

class MyButton : public RP2040::SioPin{
  friend class gui::ViewController;
public:

  enum ButtonEventType{
    Released,
    Pressed,
    Clicked,
    Held,
    RepeatClicked,
    DoubleClicked,
  };

  struct ButtonEvent{
    ButtonEventType type;
    absolute_time_t event_time;
    MyButton *button;
  };


  MyButton(uint pin, ::Polarity polarity=::Polarity::ACTIVE_LOW, uint32_t repeat_time=200, uint32_t hold_time=1000) :
  SioPin(pin, polarity), repeat_time(repeat_time), hold_time(hold_time) {
    MyButton **btnList = &firstButton;
    while(*btnList) btnList = &(*btnList)->nextButton;
      *btnList = this;
  };
  ~MyButton(){
    MyButton **btnList = &firstButton;
    while(*btnList && *btnList != this) btnList = &(*btnList)->nextButton;
    *btnList = this->nextButton;
  }

  MyButton( const MyButton& ) = delete; // non construction-copyable
  MyButton& operator=( const MyButton& ) = delete; // non copyable


  static MyButton *get_button_on_pin(uint pin);

protected:
  void onInterrupt(uint32_t flags) override;
private:
  static int64_t button_alarm_callback(alarm_id_t alarm_id, void* env);
  void set_alarm(uint time_until_ms);
  void risingEdge();
  uint pin;
  Polarity polarity;
  uint32_t repeat_time;
  uint32_t hold_time;
  bool last_state = false;
  bool held = false;

  MyButton *nextButton;
  static MyButton *firstButton;

  alarm_id_t alarm_id = 0;

  static Queue<ButtonEvent> event_queue;
};