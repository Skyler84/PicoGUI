#include "gui/widgets/divider_widget.hpp"


#define PRINT_SIZE(size) printf("[DIV] (%s) {%d,%d}\n", #size, size.w, size.h)


using namespace gui;

// bool DividerWidget::relayout(){
//     Size2 offset{};
//     Size2 rs{};

//     auto printSize = [](const Size2 s) { printf("[DIV] {%d,%d}\n", s.w, s.h); };

//     int numGrowing = 0;
//     Size2 gap{};
//     gap = accumulate({0,0}, {-getGap(), -getGap()});
//     PRINT_SIZE(offset);
//     PRINT_SIZE(gap);
//     if(m_children)
//         rs = accumulate(rs, gap);
//     gap = {-gap.w, -gap.h};
//     PRINT_SIZE(gap);
    
//     for(Widget *child = m_children; child; child = child->getNextChild()){
//         rs = accumulate(rs, child->getRequiredSize());
//         if(growable(*child))
//             numGrowing++;
//         rs = accumulate(rs, gap);
//     }
//     //if size requirements have changed, stop here and come back later
//     if(setRequiredSize(rs))
//         return true;

//     Size2 excessSize = excess(rs);
    
//     //usable size we have
//     rs = getSize();
//     for(Widget *child = m_children; child; child = child->getNextChild()){
//         Point2 pos{
//             getPos().x+offset.w,
//             getPos().y+offset.h,
//         };
//         Size2 addSize;
//         if(growable(*child)){
//             addSize = {excessSize.w/numGrowing, excessSize.h/numGrowing};
//             printf("grow (%p) {%d,%d} %d {%d,%d}\n", child, excessSize.w, excessSize.h, numGrowing, addSize.w, addSize.h);
//             numGrowing--;
//             excessSize = {excessSize.w - addSize.w, excessSize.h - addSize.h};
//         }
//         Size2 size = childSize(offset, addSize, *child);
//         PRINT_SIZE(size);
//         child->setBox({pos, size});
//         if((child->needsRelayout() || child->childNeedsRelayout()) && 
//             child->relayout()){
//             printf("relayout (%p) changed\n", child);
//             return true;
//         }
//         offset = accumulate(offset, gap);
//         PRINT_SIZE(offset);
//     }
//     relayoutDone();
//     printf("[VDWIDG] relayout() {%d,%d} done\n", getSize().w, getSize().h);
//     return false;
// }

void VDividerWidget::redraw(Graphics &graphics, bool){
    // printf("[VDWIDG] redraw()\n");
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
        if(child->canGrowH())
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
        if(child->canGrowH()){
            addHeight = excessHeight/numGrowing;
            printf("grow (%p) %d %d %d\n", child, excessHeight, numGrowing, addHeight);
            numGrowing--;
            excessHeight -= addHeight;
        }
        Size2 size{
            rs.w, 
            std::min(getSize().h - yOff, child->getRequiredSize().h + addHeight)
        };
        if((child->setBox({pos, size}) ||
            child->needsRelayout() || child->childNeedsRelayout()) && 
            child->relayout()){
            printf("relayout (%p) changed\n", child);
            return true;
        }
        yOff += size.h + getGap();
    }
    relayoutDone();
    printf("[VDWIDG] relayout() {%d,%d} done\n", getSize().w, getSize().h);
    return false;
}

void HDividerWidget::redraw(Graphics &graphics, bool){
    // printf("[HDWIDG] redraw()\n");
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
