#pragma once

#include "gui/widgets/widget.hpp"

namespace gui
{
    class HDividerWidget : public Widget{
    public:
    HDividerWidget(Widget *parent = nullptr, Rectangle r = {}) : Widget(parent, r){
        setGrow(true);
     }
    ~HDividerWidget(){};
    size_t addWidget(Widget *w){
        printf("[HDWIDG] addWidget(%p)\n", w);
        w->setParent(this);
        size_t i = 0;
        if(m_children){
            printf("1\n");
            Widget *child = m_children;
            printf("1a\n");
            ++i;
            printf("1aa\n");
            while(child->getNextChild()){
                printf("2\n");
                ++i;
                child = child->getNextChild();
            }
            // printf("3\n");
            child->setNextChild(w);
        }else{
            printf("4\n");
            m_children = w;
        }
        requiresRelayout();
        printf("[HDWIDG] addWidget() done\n");
        return i;
    }
    protected:
        void redraw(Graphics&, bool) override;
        bool relayout() override;
    private:
    Widget *m_children = nullptr;
    };

    class VDividerWidget : public Widget{
    public:
    VDividerWidget(Widget *parent = nullptr, Rectangle r = {}) : Widget(parent, r){
        setGrow(true);
    }
    ~VDividerWidget(){};
    size_t addWidget(Widget *w){
        printf("[VDWIDG] addWidget(%p)\n", w);
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
        return i;
    }
    protected:
        void redraw(Graphics&, bool) override;
        bool relayout() override;
    private:
    Widget *m_children = nullptr;
    };
} // namespace gui
