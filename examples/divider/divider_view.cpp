#include "examples/divider_view.hpp"

DividerView::DividerView(gui::View *parent) : View(parent){
    ui.hdivider.addWidget(&ui.lvdivider);
    ui.hdivider.addWidget(&ui.rvdivider);
    ui.lvdivider.addWidget(&ui.ltext);
    ui.rvdivider.addWidget(&ui.rtext1);
    ui.rvdivider.addWidget(&ui.rtext2);

    ui.ltext.setText("Left");
    ui.rtext1.setText("Right first");
    ui.rtext2.setText("Right second");
    ui.ltext.setFontSize(2);
    ui.rtext1.setFontSize(2);
    ui.rtext2.setFontSize(2);
    setWidget(&ui.hdivider);
}

void DividerView::handleInput(InputEvent ev){
    View::handleInput(ev);
}