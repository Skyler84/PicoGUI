#pragma once

struct InputEvent{
    enum EventType{
        NONE,
        ButtonRelease,
        ButtonPress,
        ButtonClick,
        ButtonHold,
        ButtonRepeatClick,
        AnalogChange, /*used for joysticks etc*/
        EncoderPlus,
        EncoderMinus,
    } type;
    void *data;
};