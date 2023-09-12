#include "examples/all_view.hpp"

#include "gui/view_controller.hpp"

#include "drivers/button.hpp"
#include "buttons.hpp"


AllView::AllView(gui::View *parent) : 
  View(parent),
  tv{this},
  dv{this},
  lv{this},
  qv{this}{
  ui.div.addWidget(&ui.header);
  ui.header.setText("All Examples");
  ui.header.setFontSize(3);
  ui.div.addWidget(&ui.list);

  ui.list.addItem(&ui.li_text);
  ui.li_text.setText("Text example");
  ui.li_text.setFontSize(2);
  ui.li_text.setUserData(&tv);
  
  ui.list.addItem(&ui.li_dividier);
  ui.li_dividier.setText("Divider example");
  ui.li_dividier.setFontSize(2);
  ui.li_dividier.setUserData(&dv);
  
  ui.list.addItem(&ui.li_list);
  ui.li_list.setText("List example");
  ui.li_list.setFontSize(2);
  ui.li_list.setUserData(&lv);
  
  ui.list.addItem(&ui.li_qrcode);
  ui.li_qrcode.setText("QR code example");
  ui.li_qrcode.setFontSize(2);
  ui.li_qrcode.setUserData(&qv);

  setWidget(&ui.div);
}

void AllView::handleInput(InputEvent ev){
  bool handled = false;
  MyButton *b = (MyButton*)ev.data;
  switch(ev.type){
    case InputEvent::ButtonClick:{
      switch(b->get_pin()){
        case Buttons::A:{
          if(ui.list.getSelectedRow() != -1);
            selectApplication(ui.list.getSelectedRow());
          break;
        }
        case Buttons::B:{
          if(ui.list.getSelectedRow() >= 0){
            ui.list.setFocused(false);
            ui.list.setSelectedRow(-1);
          }
          else
            close();
          break;
        }
        case Buttons::X:{
          ui.list.setSelectedRow(std::max<int>(ui.list.getSelectedRow()-1, -1)); 
          ui.list.setFocused(ui.list.getSelectedRow() != -1);
          break;
        }
        case Buttons::Y:{
          ui.list.setSelectedRow(std::min<int>(ui.list.getSelectedRow()+1, ui.list.getNumItems()-1)); 
          ui.list.setFocused(ui.list.getSelectedRow() != -1);
          break;
        }
      }
      handled = true;
      break;
    }
  }
  if(!handled)
    View::handleInput(ev);
}

void AllView::selectApplication(size_t idx)
{
  Widget *w = ui.list.getItemAt(idx);
  if (!w){
    printf("No item\n");
    return;
  }
  void *d = w->getUserData();
  if (!d){
    printf("No user data\n");
    return;
  }
  setChildView((gui::View*)d);
  auto &controller = gui::ViewController::get();
  controller.set_view((gui::View*)d);
}