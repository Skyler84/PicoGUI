#include <cstdint>
#include <stdio.h>
#include "drivers/button.hpp"

#include "pico/stdio.h"
#include "pico/stdlib.h"

#define BUTTON_PRINT_PRE "[BUTTON] "
#define DEBUG_PRINTF(...) printf("[BUTTON] " __VA_ARGS__)
// #define DEBUG_PRINTF(...) 



MyButton *MyButton::firstButton = nullptr;
Queue<MyButton::ButtonEvent> MyButton::event_queue{32};

void gpio_event_string(char *buf, uint32_t events);


MyButton *MyButton::get_button_on_pin(uint pin){
  return static_cast<MyButton*>(getSioPin(pin));
}

void MyButton::onInterrupt(uint32_t flags)
{
  static char event_str[128];
  // Put the GPIO event(s) that just happened into event_str
  // so we can print it
  gpio_event_string(event_str, flags);
  if(flags == 0b1100)
    return; //do nothing as it's a bounce
  this->set_alarm(DEBOUNCE_TIME);

  DEBUG_PRINTF("GPIO %d %s\n", get_pin(), event_str);
}

int64_t MyButton::button_alarm_callback(alarm_id_t alarm_id, void* env){
  MyButton *button = (MyButton*)env;
  DEBUG_PRINTF("alarm %d callback, btn %d\n", alarm_id, button->pin);
  // button->alarm_id = 0; 
  //check state change
  bool val = button->raw();
  bool changed = val != button->last_state;
  button->last_state = val;
  if(changed){
    if(val){
      //pressed
      DEBUG_PRINTF("Pressed\n");
      event_queue.try_add(ButtonEvent{ButtonEventType::Pressed, button});
      DEBUG_PRINTF("Alarm rescheduled for 1s\n");
      return button->hold_time*1000;
    }else{
      //released
      DEBUG_PRINTF("Released\n");
      event_queue.try_add(ButtonEvent{ButtonEventType::Released, button});
      if(!button->held){
        DEBUG_PRINTF("Clicked\n");
        event_queue.try_add(ButtonEvent{ButtonEventType::Clicked, button});
      }
    }
    button->held = false;
  }else if(val){
    //held
    if(!button->held){
      DEBUG_PRINTF("Held\n");
      button->held = true;
      event_queue.try_add(ButtonEvent{ButtonEventType::Held, button});
    }
    if(button->repeat_time){
      DEBUG_PRINTF("Repeat Clicked\n");
      event_queue.try_add(ButtonEvent{ButtonEventType::RepeatClicked, button});
      DEBUG_PRINTF("Alarm rescheduled for 0.2s\n");
      return button->repeat_time*1000;
    }
  }
  DEBUG_PRINTF("Alarm %d cleared\n", alarm_id);
  button->alarm_id = 0;
  return 0;
}

void MyButton::set_alarm(uint time_until_ms){
  if(this->alarm_id){
    DEBUG_PRINTF("cancelling alarm %d\n", this->alarm_id);
    cancel_alarm(this->alarm_id);
  }
  this->alarm_id = 0;
  DEBUG_PRINTF("setting alarm in %dms for button %d\n", time_until_ms, this->pin);
  this->alarm_id = add_alarm_at(make_timeout_time_ms(time_until_ms), button_alarm_callback, this, false);
  DEBUG_PRINTF("alarm %d set\n", this->alarm_id);
}



static const char *gpio_irq_str[] = {
        "LEVEL_LOW",  // 0x1
        "LEVEL_HIGH", // 0x2
        "EDGE_FALL",  // 0x4
        "EDGE_RISE"   // 0x8
};

void gpio_event_string(char *buf, uint32_t events) {
    for (uint i = 0; i < 4; i++) {
        uint mask = (1 << i);
        if (events & mask) {
            // Copy this event string into the user string
            const char *event_str = gpio_irq_str[i];
            while (*event_str != '\0') {
                *buf++ = *event_str++;
            }
            events &= ~mask;

            // If more events add ", "
            if (events) {
                *buf++ = ',';
                *buf++ = ' ';
            }
        }
    }
    *buf++ = '\0';
}