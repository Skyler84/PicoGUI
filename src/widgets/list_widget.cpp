#include "gui/widgets/list_widget.hpp"

using namespace gui;

ListWidget::ListWidget(Widget *parent, Rectangle r): 
Widget(parent, r), m_selectedRow{0}, m_vScroll{0}, m_children{nullptr}{
    setGrow(true);
}

size_t ListWidget::addItem(Widget *w){
    return addItemAt(w, -1);
}

size_t ListWidget::addItemAt(Widget *w, size_t idx){
    if(!w)
        return -1;
    printf("[LWIDG] addItemAt(%p, %d)\n", w, idx);
    assert(itemPosition(w) == -1);
    size_t i = 0;
    int yOff = 0;
    w->setNextChild(nullptr);
    w->setParent(this);
    if(m_children){
        Widget *child = m_children;
        while(child->getNextChild() && i <= idx){
            ++i;
            child = child->getNextChild();
        }
        child->setNextChild(w);
    }else{
        m_children = w;
    }

    requiresRelayout();
    requiresRedraw();

    return i;
}

Widget *ListWidget::getItemAt(size_t idx){
    Widget *w = m_children;
    while(w && idx--){
        w = w->getNextChild();
    }
    return w;
}

Widget *ListWidget::removeItemAt(size_t idx){
    return removeItem(getItemAt(idx));
}

Widget *ListWidget::removeItem(Widget* w){
    if(!w)
        return nullptr;
    Widget *child = m_children;
    if(child == w){
        m_children = child->getNextChild();
    }else{
        size_t i = 0;
        while(child->getNextChild() != w && child){
            child = child->getNextChild();
        }
        if(!child)
            return nullptr;
        //should ALWAYS be valid pointer as we checkd already?
        Widget *tmp = child->getNextChild();
        assert(tmp);
        child->setNextChild(tmp->getNextChild());
        child = tmp;
    }
    child->setNextChild(nullptr);
    child->setParent(nullptr);
    requiresRelayout();
    requiresRedraw();
    return child;
}

size_t ListWidget::itemPosition(Widget *w){
    size_t i = 0;
    Widget *child = m_children;
    while(child && child != w){
        i++;
        child = child->getNextChild();
    }

    if(!child)
        return -1;
    return i;
}

void ListWidget::setSelectedRow(size_t idx){
    printf("[LWIDG] setSelectedRow(%d)\n", idx);
    Widget *w = getItemAt(m_selectedRow);
    printf("[LWIDG] %p\n", w);
    if(w)
        w->setFocused(false);
    m_selectedRow = idx;
    w = getItemAt(m_selectedRow);
    if(w)
        w->setFocused(true);
    requiresRedraw();
    
}

void ListWidget::redraw(Graphics &g, bool){
    printf("[LWIDG] redraw()\n");
    Widget *child, *nextchild = m_children;
    auto mpos = getPos();
    auto msize = getSize();
    int i = 0;
    while(nextchild){
        child = nextchild;
        nextchild = child->getNextChild();
        printf("[LWIDG] redraw() %p\n", child);
        // child->setFocused()
        auto cpos = child->getPos();
        auto csize = child->getSize();
        //check is within bounds
        if((cpos.x < mpos.x) || ((cpos.x + csize.w) > (mpos.x + msize.w)))
            continue;
        printf("[LWIDG] if((%d < %d) || (%d > %d))\n", cpos.y, mpos.y, (cpos.y + csize.h), (mpos.y + msize.h));
        if((cpos.y < mpos.y) || ((cpos.y + csize.h) > (mpos.y + msize.h)))
            continue;
        
        printf("[LWIDG] redraw() %p yes\n", child);
        child->redraw(g);
    }
    printf("[LWIDG] redraw() done\n");
}

bool ListWidget::relayout(){
    printf("[LWIDG] relayout() {%d,%d,%d,%d}\n", getPos().x, getPos().y, getSize().w, getSize().h);

    //global yOff is size of first m_vScroll widgets
    int yOff = 0;
    size_t i = 0;
    Widget *child;
    if(!m_children)
        return false;
    
    for(child = m_children; i < m_vScroll && child; child = child->getNextChild()){
        yOff -= child->getRequiredSize().h;
        ++i;
    }
    //check if selected item would be off screen?
    for(child = m_children; child; child = child->getNextChild()){
        Point2 pos{
            getPos().x, 
            getPos().y+yOff
        };
        //TODO: respect layout rules?
        Size2 size{
            getSize().w, 
            child->getRequiredSize().h
        };
        if(child->setBox({pos, size}) && child->relayout())
            return true;
        yOff += child->getRequiredSize().h;
        ++i;
    }
    // relayoutDone();
    requiresRedraw();
    return false;
}

void ListWidget::onFocusChange(){
    Widget *w = getItemAt(m_selectedRow);
    if(!w)
        return;
    w->setFocused(hasFocus());
    requiresRedraw();
}