#pragma once

#include "hardware/gpio.h"

enum Polarity {
    ACTIVE_LOW = 0,
    ACTIVE_HIGH = 1
};

namespace RP2040
{
    class GpioPin{
    public:
        GpioPin(uint pin, Polarity pol);
        virtual void init() = 0;
        bool raw();
        void put(bool val);
        void toggle();
        uint get_pin() const { return pin; }
        Polarity getPolarity() const { return pol; }
        int getDirection() const { return dir; }
    protected:
        void setFunc(gpio_function f) { func = f;}
        void setDirection(int d) { dir = d;}
        void setPolarity(Polarity p) { pol = p; }
    private:
        uint pin;
        Polarity pol;
        int dir;
        gpio_function func;
    };
} // namespace RP2040
