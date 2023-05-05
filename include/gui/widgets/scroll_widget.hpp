#pragma once

#include "gui/widgets/widget.hpp"

namespace gui
{
    class ScrollWidget{
    public:
    protected:
        void redraw(Graphics &) override;
    private:
        bool m_hScrollEnabled;
        bool m_vScrollEnabled;
        size_t m_hScroll;
        size_t m_vScroll;

        Widget *m_child;

    };
} // namespace gui
