#pragma once

#include "drivers/rp2040/sio_pin.hpp"
#include "pico/time.h"
#include "queue.hpp"

namespace IR
{
    class LED : public RP2040::SioPin{
    public:
        LED(uint pin);
        ~LED(){}
    protected:
        // void init() override;

    };
    class Detector : public RP2040::SioPin{
    public:
        Detector(uint pin);
        ~Detector(){}
        // void init() override;
    protected:
        void onInterrupt(uint32_t flags) override;
    private:
        absolute_time_t prev_time;
        long prev_time_diff_us;
        bool prev_state;
        bool state;
    };
} // namespace IR
