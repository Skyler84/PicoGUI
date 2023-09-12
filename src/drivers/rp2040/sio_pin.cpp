#include "drivers/rp2040/sio_pin.hpp"

#include "stdio.h"

using namespace RP2040;

SioPin *SioPin::s_firstPin = nullptr;

SioPin::SioPin(uint pin, Polarity pol) : GpioPin{pin, pol} {
    SioPin **pinList = &s_firstPin;
    while(*pinList) pinList = &(*pinList)->m_nextPin;
    *pinList = this;
}

SioPin::~SioPin(){
  // TODO: fix memory leak
    // SioPin **pinList = &s_firstPin;
    // while(*pinList && ) pinList = &(*pinList)->m_nextPin;
    // *pinList = this;
}

void SioPin::init() {
  uint pin = get_pin();
  printf("SioPin::init() %d, %s\n", pin, getDirection()==GPIO_IN?"IN":"OUT");
    GpioPin::init();
    if(getDirection() == GPIO_IN){
        gpio_set_pulls(get_pin(), 
          getPolarity() == Polarity::ACTIVE_LOW, 
          getPolarity() == Polarity::ACTIVE_HIGH);
        gpio_set_irq_enabled_with_callback(pin, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, SioPin::interrupt);
    } else {
        gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_2MA);
        put(0);
    }
}

SioPin *SioPin::getSioPin(uint pin){
  SioPin **pinList = &s_firstPin;
  while(SioPin* p = *pinList) {
    if(p->get_pin() == pin)
      return p;
    pinList = &p->m_nextPin;
  }
  return nullptr;
}

void SioPin::interrupt(uint pin, uint32_t flags){
    if(auto s = getSioPin(pin))
        s->onInterrupt(flags);
}