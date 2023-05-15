#include "gui/widgets/divider_widget.hpp"

using namespace gui;

void VDividerWidget::redraw(Graphics &graphics, bool){
    printf("[VDWIDG] redraw()\n");
    Widget *w = m_children;
    while(w){
        w->redraw(graphics);
        w->redrawDone();
        graphics.set_pen(255);
        if(w->getNextChild() && getSeparator()){
            graphics.rectangle({{getPos().x+1, w->getPos().y+w->getSize().h + getSpacing()}, {getPos().x+getSize().w-1, w->getPos().y+w->getSize().h + getSpacing()+getSeparator()}});
        }
        w = w->getNextChild();
    }
}

bool VDividerWidget::relayout(){
    printf("[VDWIDG] relayout() {%d,%d}\n", getSize().w, getSize().h);
    int yOff = 0;
    // required size
    Size2 rs{};

    int numGrowing = 0;
    if(m_children)
        rs.h -= getGap();
    for(Widget *child = m_children; child; child = child->getNextChild()){
        rs.w = std::max(rs.w, child->getRequiredSize().w);
        rs.h += child->getRequiredSize().h;
        if(child->canGrow())
            numGrowing++;
        rs.h += getGap();
    }
    //if size requirements have changed, stop here and come back later
    if(setRequiredSize(rs))
        return true;
    int excessHeight = std::max(getSize().h - rs.h, 0);
    //usable size we have
    rs = getSize();
    for(Widget *child = m_children; child; child = child->getNextChild()){
        Point2 pos{
            getPos().x, 
            getPos().y+yOff
        };
        int addHeight = 0;
        if(child->canGrow()){
            addHeight = excessHeight/numGrowing;
            printf("grow %d %d %d\n", excessHeight, numGrowing, addHeight);
            numGrowing--;
            excessHeight -= addHeight;
        }
        Size2 size{
            rs.w, 
            std::min(getSize().h - yOff, child->getRequiredSize().h + addHeight)
        };
        if((child->setBox({pos, size}) ||
            child->needsRelayout()) && 
            child->relayout())
            return true;
        yOff += size.h + getGap();
    }
    relayoutDone();
    printf("[VDWIDG] relayout() {%d,%d}\n", getSize().w, getSize().h);
    return false;
}

void HDividerWidget::redraw(Graphics &graphics, bool){
    printf("[HDWIDG] redraw()\n");
    Widget *w = m_children;
    while(w){
        w->redraw(graphics);
        w->redrawDone();
        graphics.set_pen(255);
        if(w->getNextChild()){
            graphics.rectangle({{w->getPos().x + w->getSize().w + getSpacing(), getPos().y + 1}, {w->getPos().x + w->getSize().w + getSpacing()+getSeparator(), getPos().y + getSize().h - 1}});
        }
        w = w->getNextChild();
    }    
}


bool HDividerWidget::relayout(){
    printf("[HDWIDG] relayout() {%d,%d}\n", getSize().w, getSize().h);
    int xOff = 0;
    // required size
    Size2 rs{};

    int numGrowing = 0;
    if(m_children)
        rs.w -= getGap();
    for(Widget *child = m_children; child; child = child->getNextChild()){
        rs.h = std::max(rs.h, child->getRequiredSize().h);
        rs.w += child->getRequiredSize().w;
        if(child->canGrow())
            numGrowing++;
        rs.w += getGap();
    }
    //if size requirements have changed, stop here and come back later
    if(setRequiredSize(rs))
        return true;
    int excessWidth = std::max(getSize().w - rs.w, 0);
    //usable size we have
    rs = getSize();
    for(Widget *child = m_children; child; child = child->getNextChild()){
        Point2 pos{
            getPos().x + xOff, 
            getPos().y
        };
        int addWidth = 0;
        if(child->canGrow()){
            addWidth = excessWidth/numGrowing;
            printf("grow %d %d %d\n", excessWidth, numGrowing, addWidth);
            numGrowing--;
            excessWidth -= addWidth;
        }
        Size2 size{
            std::min(getSize().w - xOff, child->getRequiredSize().w + addWidth),
            rs.h
        };
        if((child->setBox({pos, size}) ||
            child->needsRelayout()) && 
            child->relayout())
            return true;
        xOff += size.w + getGap();
    }
    relayoutDone();
    printf("[HDWIDG] relayout() {%d,%d}\n", getSize().w, getSize().h);
    return false;
}
