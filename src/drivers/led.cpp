#include "drivers/led.hpp"


LED::LED(uint pin, Polarity p) : RP2040::PwmPin{pin, p}{
    setDirection(GPIO_OUT);

}
