#pragma once

#include "gui/widgets/widget.hpp"

#include <algorithm>

namespace gui
{
    /* abscract */ class DividerWidget : public Widget{
    public:
        DividerWidget(Widget *parent = nullptr, Rectangle r = {}) : Widget(parent, r){
            setGrow(true);
        }
        int getSpacing() const { return m_spacing; }
        int getGap() const { return m_separator + 2*m_spacing; }
        int getSeparator() const { return m_separator; }
        void setSpacing(int s) { m_spacing = s; }
        void setSeparator(int s) { m_separator = s; }


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
        bool relayout() override = 0;
        // virtual Point2 convert(Point2) = 0;
        // virtual Point2 convert(Size2) = 0;
        virtual Size2 accumulate(Size2, Size2) = 0;
        virtual bool growable(const Drawable &) = 0;
        virtual Size2 excess(Size2) = 0;
        virtual Size2 childSize(Size2 off, Size2 add, const Drawable&d) = 0;
        Widget *m_children = nullptr;
    private:
        int m_spacing = 1;
        int m_separator = 1;
    };
    class HDividerWidget : public DividerWidget{
    public:
        HDividerWidget(Widget *parent = nullptr, Rectangle r = {}) : DividerWidget(parent, r){ }
        ~HDividerWidget(){};
    protected:
        void redraw(Graphics&, bool) override;
        bool relayout() override;
        Size2 accumulate(Size2 a, Size2 b) override { return {a.w+b.w, std::max(a.h,b.h)}; }
        bool growable(const Drawable&d) override { return d.canGrowH(); }
        Size2 excess(Size2 s) override { return {std::max(getSize().w - s.w, 0), 0}; }
        Size2 childSize(Size2 off, Size2 add, const Drawable&d) override { return {std::min(getSize().w - off.w, d.getRequiredSize().w+add.w), getSize().h}; }
    private:
    };

    class VDividerWidget : public DividerWidget{
    public:
        VDividerWidget(Widget *parent = nullptr, Rectangle r = {}) : DividerWidget(parent, r){ }
        ~VDividerWidget(){};
    protected:
        void redraw(Graphics&, bool) override;
        bool relayout() override;
        Size2 accumulate(Size2 a, Size2 b) override { return {std::max(a.w,b.w), a.h+b.h}; }
        bool growable(const Drawable&d) override { return d.canGrowV(); }
        Size2 excess(Size2 s) override { return {0, std::max(getSize().h - s.h, 0)}; }
        Size2 childSize(Size2 off, Size2 add, const Drawable&d) override { return {getSize().w, std::min(getSize().h - off.h, d.getRequiredSize().h+add.h)}; }
    private:
    };
} // namespace gui
