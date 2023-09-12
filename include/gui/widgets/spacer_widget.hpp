#pragma once

#include "widget.hpp"

namespace gui
{
    class SpacerWidget : public Widget{
    protected:
        void redraw(Graphics&, bool) override {
            printf("SpacerWidget::redraw()\n");
        }
    };
    class HSpacerWidget : public Widget{};
    class VSpacerWidget : public Widget{};

} // namespace gui
