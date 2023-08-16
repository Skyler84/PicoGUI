#include "drivers/rp2040/gpio_pin.hpp"

using namespace RP2040;

GpioPin::GpioPin(uint pin, Polarity pol): pin{pin}, pol{pol}
{

}

void GpioPin::init(){
    gpio_init(pin);
}

bool GpioPin::raw(){
  if(pol == Polarity::ACTIVE_LOW){
    return !gpio_get(pin);
  } else {
    return gpio_get(pin);
  }
}

void GpioPin::put(bool v){
  if(pol == Polarity::ACTIVE_LOW){
    gpio_put(pin, !v);
  } else {
    gpio_put(pin, v);
  }
}
