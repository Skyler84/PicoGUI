#include "gui/drawable.hpp"
#include "gui/view_controller.hpp"

using namespace gui;


void Drawable::clear(gui::Graphics &graphics){
    gui::Color col = graphics.color;
    gui::Color BG = ViewController::get().getBackgroundColor();
    graphics.set_pen(BG);
    graphics.rectangle(m_box);   
    graphics.set_pen(col); 
}
void Drawable::clear(gui::Graphics &graphics, gui::Color BG){
    gui::Color col = graphics.color;
    graphics.set_pen(BG);
    graphics.rectangle(m_box);   
    graphics.set_pen(col); 
}