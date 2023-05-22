#pragma once

#include "gpio_pin.hpp"

namespace RP2040
{

    class SioPin : public GpioPin{
    public:
        SioPin(uint pin, Polarity polarity=Polarity::ACTIVE_LOW);
        ~SioPin();
        virtual void init() override;

        static SioPin *getSioPin(uint pin);  
        static void init_all(){
            SioPin **pinList = &s_firstPin;
            while(*pinList) {
                (*pinList)->init();
                pinList = &(*pinList)->m_nextPin;
            }
        }
    protected:
        virtual void onInterrupt(uint32_t flags){};
    private:
        uint32_t repeat_time; 
        bool interruptsEnabled = true;
        uint32_t interruptFlags = 0;

        static void interrupt(uint pin, uint32_t flags);
        static SioPin *s_firstPin;
        SioPin *m_nextPin = nullptr;
    };
} // namespace RP2040
