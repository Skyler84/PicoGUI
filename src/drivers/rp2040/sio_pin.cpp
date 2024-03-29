#include "drivers/rp2040/sio_pin.hpp"

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

void SioPin::init(){
  gpio_init(get_pin());
  gpio_set_pulls(get_pin(), 
    getPolarity() == Polarity::ACTIVE_LOW, 
    getPolarity() == Polarity::ACTIVE_HIGH);
  gpio_set_irq_enabled_with_callback(get_pin(), GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, SioPin::interrupt);
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