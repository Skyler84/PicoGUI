#pragma once

#include "gui/widgets/widget.hpp"

namespace gui
{
    /* abscract */ class DividerWidget : public Widget{
    public:
        DividerWidget(Widget *parent = nullptr, Rectangle r = {}) : Widget(parent, r){
            setGrow(true);
        }
        uint getSpacing() const { return m_spacing; }
        uint getGap() const { return m_separator + 2*m_spacing; }
        uint getSeparator() const { return m_separator; }
        void setSpacing(uint s) { m_spacing = s; }
        void setSeparator(uint s) { m_separator = s; }


        size_t addWidget(Widget *w){
            printf("[DIV] addWidget(%p)\n", w);
            w->setParent(this);
            size_t i = 0;
            if(m_children){
                Widget *child = m_children;
                ++i;
                while(child->getNextChild()){
                    ++i;
                    child = child->getNextChild();
                }
                child->setNextChild(w);
            }else{
                m_children = w;
            }
            requiresRelayout();
            printf("[DIV] addWidget() done\n");
            return i;
        }
        Widget *removeWidget(Widget *w){
            Widget *ws = m_children;
            while(ws && ws != w){
                ws = ws->getNextChild();
            }
            if(ws)
                requiresRelayout();
            return ws;
        }


    protected:
        // virtual Point2 convert(Point2) = 0;
        // virtual Point2 convert(Size2) = 0;
        Widget *m_children = nullptr;
    private:
        uint m_spacing = 1;
        uint m_separator = 1;
    };
    class HDividerWidget : public DividerWidget{
    public:
        HDividerWidget(Widget *parent = nullptr, Rectangle r = {}) : DividerWidget(parent, r){ }
        ~HDividerWidget(){};
    protected:
        void redraw(Graphics&, bool) override;
        bool relayout() override;
    private:
    };

    class VDividerWidget : public DividerWidget{
    public:
        VDividerWidget(Widget *parent = nullptr, Rectangle r = {}) : DividerWidget(parent, r){ }
        ~VDividerWidget(){};
    protected:
        void redraw(Graphics&, bool) override;
        bool relayout() override;
    private:
    };
} // namespace gui
