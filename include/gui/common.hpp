#pragma once
#include "drivers/st7789/st7789.hpp"

#include "pico_graphics.hpp"
namespace gui
{
    using Display = pimoroni::ST7789;
    using Graphics = pimoroni::PicoGraphics_PenRGB332;
    using Color = pimoroni::Pen;
    using Font = bitmap::font_t;
} // namespace gui
