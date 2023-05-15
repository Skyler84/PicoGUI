#include "drivers/rp2040/sio_pin.hpp"

using namespace RP2040;

SioPin *SioPin::s_firstPin = nullptr;

SioPin::SioPin(uint pin, Polarity pol) : pin{pin}, polarity{pol} {
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

bool SioPin::raw(){
  if(polarity == Polarity::ACTIVE_LOW){
    return !gpio_get(pin);
  } else {
    return gpio_get(pin);
  }
}

void SioPin::put(bool v){
  if(polarity == Polarity::ACTIVE_LOW){
    gpio_put(pin, !v);
  } else {
    gpio_put(pin, v);
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