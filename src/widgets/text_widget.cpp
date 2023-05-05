#include "gui/widgets/text_widget.hpp"
#include "gui/view_controller.hpp"
#include "font6mono_data.hpp"

using namespace gui;

TextWidget::TextWidget(Widget *parent, const Point2 &pos) : 
Widget(parent, pos){
    m_font = &font6mono;    
    setText("");
    setFontSize(1);
    setFontColor(255);
}

void TextWidget::setText(const std::string_view &s){
    this->m_textContent = s;
    recalcSize();
    requiresRedraw();
}

std::string_view TextWidget::getText() const{
    return this->m_textContent;
}

void TextWidget::setFontColor(gui::Color c){
    this->m_textColor = c;
    requiresRedraw();
}

void TextWidget::setFontSize(int size){
    this->m_textScale = size;
    recalcSize();
}

void TextWidget::setFont(const gui::Font *font){
    this->m_font = font;
}

void TextWidget::redraw(Graphics& graphics, bool){
    // printf("[TWIDG] redraw() %p focus %d\n", this, hasFocus());
    if(hasFocus()){
        clear(graphics, this->m_textColor);
        graphics.set_pen(ViewController::get().getBackgroundColor());
    }else{
        clear(graphics);
        graphics.set_pen(this->m_textColor);
    }
    graphics.set_font(this->m_font);
    graphics.text(m_textContent, getPos(), -1, m_textScale);
}

void TextWidget::recalcSize(){
    Size2 size{
        bitmap::measure_text(m_font, m_textContent, m_textScale),
        (m_font->height+1) * m_textScale,
    };
    if(this->setRequiredSize(size))
        requiresRelayout();
    requiresRedraw();
}