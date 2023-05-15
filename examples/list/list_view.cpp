#include "examples/list_view.hpp"

#include "drivers/button.hpp"
#include "buttons.hpp"

ListView::ListView(gui::View *parent) : View(parent){
    ui.list.addItem(&ui.t1);
    ui.list.addItem(&ui.t2);
    ui.list.addItem(&ui.t3);
    ui.t1.setText("Row item 1");
    ui.t2.setText("Row item 2");
    ui.t3.setText("Use X and Y to scroll!");
    setWidget(&ui.list);
}

void ListView::handleInput(InputEvent ev){
    bool handled = false;
  MyButton *b = (MyButton*)ev.data;
  switch(ev.type){
    case InputEvent::ButtonClick:{
      switch(b->get_pin()){
        case Buttons::A:{
          if(ui.list.getSelectedRow() != -1);
            // selectApplication(ui.list.getSelectedRow());
            ;
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