#pragma once

#include "gpio_pin.hpp"

namespace RP2040
{
    class PwmPin : public GpioPin{
    public:
        PwmPin(uint pin, Polarity pol) : GpioPin{pin, pol}{
            setFunc(GPIO_FUNC_PWM);
        }
    };
} // namespace RP2040
