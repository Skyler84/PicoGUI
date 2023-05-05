#pragma once

#include "widget.hpp"
#include <string_view>

namespace gui
{
    class TextWidget : public Widget{
    public:
        TextWidget(Widget *parent = nullptr, const Point2 &pos = {});
        void setText(const std::string_view&);
        std::string_view getText() const;
        void setFontSize(int scale);
        void setFontColor(gui::Color c);
        void setFont(const gui::Font*);
        // void setHighlight(bool h) { m_isHighlighted = h; }
        void redraw(Graphics&, bool = false);
    protected:
    private:
        std::string m_textContent;
        const gui::Font *m_font;
        gui::Color m_textColor;
        int m_textScale;

        void recalcSize();
    };
} // namespace gui
