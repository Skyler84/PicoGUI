#pragma once

#include "hardware/gpio.h"

enum Polarity {
    ACTIVE_LOW = 0,
    ACTIVE_HIGH = 1
};

namespace RP2040
{

    class SioPin{
    public:
        SioPin(uint pin, Polarity polarity=Polarity::ACTIVE_LOW);
        ~SioPin();
        virtual void init(){
            gpio_set_function(pin, GPIO_FUNC_SIO);
            gpio_set_dir(pin, dir);
            if(dir == GPIO_IN){
                if (polarity == Polarity::ACTIVE_LOW) {
                    gpio_pull_up(pin);
                } else {
                    gpio_pull_down(pin);
                }
                gpio_set_irq_enabled_with_callback(pin, 0xc, true, interrupt);
            } else {
                gpio_set_drive_strength(pin, GPIO_DRIVE_STRENGTH_2MA);
            }
        }

        bool raw();
        void put(bool val);
        void toggel();
        uint get_pin() const {return pin;}
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
        void setDirection(int d) { dir = d; }
    private:
        uint pin;
        int dir = GPIO_IN;
        Polarity polarity;
        uint32_t repeat_time; 
        bool interruptsEnabled = true;
        uint32_t interruptFlags = 0;

        static void interrupt(uint pin, uint32_t flags);
        static SioPin *s_firstPin;
        SioPin *m_nextPin = nullptr;
    };
} // namespace RP2040
