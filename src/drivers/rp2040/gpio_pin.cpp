#include "drivers/rp2040/gpio_pin.hpp"

using namespace RP2040;

GpioPin::GpioPin(uint pin, Polarity p) : pin{pin}, pol{pol} {}

void GpioPin::init(){
    gpio_set_function(pin, func);
    gpio_set_dir(pin, dir);
    if(dir == GPIO_OUT){
        if(pol == Polarity::ACTIVE_LOW)
            gpio_set_outover(pin, IO_BANK0_GPIO0_CTRL_OUTOVER_VALUE_INVERT);
        else
            gpio_set_outover(pin, IO_BANK0_GPIO0_CTRL_OUTOVER_VALUE_NORMAL);
    } else {
        if(pol == Polarity::ACTIVE_LOW)
            gpio_set_inover(pin, IO_BANK0_GPIO0_CTRL_INOVER_VALUE_INVERT);
        else
            gpio_set_inover(pin, IO_BANK0_GPIO0_CTRL_INOVER_VALUE_NORMAL);
    }
    gpio_put(pin, 0);
}

bool GpioPin::raw(){
    return gpio_get(pin);
}

void GpioPin::put(bool v){
    gpio_put(pin, v);
}
