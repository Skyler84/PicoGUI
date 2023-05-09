#pragma once

namespace gui{
    class Widget;
}

// #include "gui/view_controller.hpp"
#include "gui/drawable.hpp"
#include "gui/shapes.hpp"

namespace gui
{

    enum class WidgetType{
        UserWidget,
        TextWidget,
        ListWidget,
        HDividerWidget,
        VDividerWidget,
    };

    class Widget : public Drawable{
    public:
        Widget():
            Drawable{nullptr, {}} {}
        Widget(Drawable *parent, const gui::Rectangle& box):
            Drawable{parent, box} {}
        Widget(Drawable *parent, const gui::Point2& pos):
            Drawable{parent, pos} {}
        template<class T>
        void setUserData(T *data) {
            m_userData = reinterpret_cast<void*>(data);
            printf("[WIDG] set user data (%p),%p\n", this, m_userData);
        }
        template<class T>
        T* getUserData() {
            printf("[WIDG] get user data (%p),%p\n", this, m_userData);
            return reinterpret_cast<T*>(m_userData);
        }

        bool hasFocus() const{
            return m_hasFocus;
        }
        void setFocused(bool focus){
            printf("[WIDG] (%p) focus %d\n", this, focus);
            m_hasFocus = focus;
            onFocusChange();
        }
        void redrawRecursive(Graphics &g){
            if(needsRedrawing() || childNeedsRedrawing()){
                redraw(g);
                redrawDone();
            }
            if(m_nextChild)
                m_nextChild->redrawRecursive(g);
        }
        Widget *getNextChild() const { 
            // assert(m_nextChild != this);
            return m_nextChild; 
        }
        void setNextChild(Widget *w) { 
            // assert(w != this);
            m_nextChild = w; 
        }
    protected:
        virtual void onFocusChange(){}
    private:
        Widget *m_nextChild = nullptr;
        void *m_userData = nullptr;
        bool m_hasFocus = false;


    };
} // namespace gui

