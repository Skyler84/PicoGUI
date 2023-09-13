#pragma once

#include "gui/widgets/widget.hpp"

namespace gui{
    class ListWidget : public Widget {
    public:
        ListWidget(Widget *parent = nullptr, Rectangle = {});
        /**
         * Adds a widget to the list
         * @param w Widget* to add
         * @return position of item (number of items in the list-11)
        */
        size_t addItem(Widget *w);
        Widget *getItemAt(size_t idx);
        Widget *removeItemAt(size_t idx);
        Widget *removeItem(Widget *w);
        size_t addItemAt(Widget *w, size_t idx);
        size_t addItemAfter(Widget *w, Widget *after);
        size_t itemPosition(Widget *w);
        /**
         * @return 
         * Next item if w in list AND has a next item
         * otherwise nullptr
        */
        Widget *getNextItem(Widget *w);
        void setSelectedRow(size_t idx);
        size_t getSelectedRow() {return m_selectedRow;};
        size_t getNumItems() {
            size_t i = 0;
            Widget *w = m_children;
            while(w){
                ++i;
                printf("[LWIDG] (%p), %d\n", w, i);
                w=w->getNextChild();
            }
            return i;
        }

        // bool handleScrollInput(InputEvent ev);

    protected:
        void redraw(Graphics&, bool = false) override;
        bool relayout() override;
        void onFocusChange() override;
    private:
        int m_selectedRow;
        int m_vScroll;
        Widget *m_children;
    };
}
