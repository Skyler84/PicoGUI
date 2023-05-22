#pragma once

#include "drivers/rp2040/pwm_pin.hpp"

class LED : public RP2040::PwmPin{
public:
    LED(uint pin, Polarity p = Polarity::ACTIVE_HIGH);
    ~LED(){}
protected:
    // void init() override;

};