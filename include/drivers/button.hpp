#pragma once

#include "hardware/gpio.h"
#include "pico/time.h"
#include "queue.hpp"

#define DEBOUNCE_TIME 15 /*ms*/

class MyButton;

namespace gui{
  class ViewController;
}

class MyButton{
  friend class gui::ViewController;
public:
  enum Polarity {
    ACTIVE_LOW = 0,
    ACTIVE_HIGH = 1
  };

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
    MyButton *button;
  };


  MyButton(uint pin, Polarity polarity=Polarity::ACTIVE_LOW, uint32_t repeat_time=200, uint32_t hold_time=1000) :
  pin(pin), polarity(polarity), repeat_time(repeat_time), hold_time(hold_time) {
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
  void init(){
    gpio_set_function(pin, GPIO_FUNC_SIO);
    gpio_set_dir(pin, GPIO_IN);
    if (polarity == Polarity::ACTIVE_LOW) {
      gpio_pull_up(pin);
    }
    else {
      gpio_pull_down(pin);
    }
    gpio_set_irq_enabled_with_callback(pin, 0xc, true, button_interrupt);
  }
  bool raw();
  uint get_pin() const {return pin;}

  static MyButton *get_button_on_pin(uint pin);
  static void init_all(){
    // printf("[BTN] init all\n");
    MyButton **btnList = &firstButton;
    while(*btnList) {
      (*btnList)->init();
      btnList = &(*btnList)->nextButton;
    }
  }

private:
  static void button_interrupt(uint gpio, uint32_t flags);
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