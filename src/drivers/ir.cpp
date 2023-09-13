#include "drivers/ir.hpp"

#include "pico/stdio.h"
#include "pico/printf.h"

using namespace IR;
using namespace RP2040;

LED::LED(uint pin) : RP2040::SioPin{pin, Polarity::ACTIVE_HIGH}{
    setDirection(GPIO_OUT);
}

#define BIT_PERIOD_US 400

Detector::Detector(uint pin) : SioPin{pin}{

}

void Detector::onInterrupt(uint32_t flags){
    // bool 
    
    bool new_state = raw();
    if(new_state == state)
        return;
    //calulate timing and therefore num bits
    absolute_time_t time = get_absolute_time();
    int time_diff_us = absolute_time_diff_us(prev_time, time);
    printf("%c %d,", '0'+state, time_diff_us);

    // time_diff_us += (BIT_PERIOD_US* 0.5);
    // int num_bits = time_diff_us/BIT_PERIOD_US;
    // if(num_bits < 10){
    //     for(int i = 0; i < num_bits; i++)
    //         printf("%c", '0'+state);
    // }else{
    //     printf("\n");
    // }

    prev_time = time;
    prev_state = state;
    state = new_state;
    prev_time_diff_us = time_diff_us;
}