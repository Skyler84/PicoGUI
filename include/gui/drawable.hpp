#pragma once

#include "gui/shapes.hpp"
#include "gui/common.hpp"

namespace gui
{

    class Drawable{
        friend class View;
        friend class ViewController;
    public:
        Drawable(Rectangle box) : m_parent{nullptr}, m_box{box}{}
        Drawable(Drawable *parent, Rectangle box) : m_parent{parent}, m_box{box}{}
        const Rectangle& getBox() const { return m_box; }
        const Point2& getPos() const { return m_box.pos; }
        const Size2& getSize() const { return m_box.size; }
        const Size2& getRequiredSize() const { return m_requiredSize; }
        bool canGrow() const { return m_grow; }
        bool canGrowH() const { return m_growH; }
        bool canGrowV() const { return m_growV; }
        void setGrow(bool grow) {
            if(m_grow == grow)
                return;
            m_grow = grow;
            requiresRelayout();
         }
        void setGrowH(bool grow) {
            if(m_growH == grow)
                return;
            m_growH = grow;
            requiresRelayout();
         }
        void setGrowV(bool grow) {
            if(m_growV == grow)
                return;
            m_growV = grow;
            requiresRelayout();
         }

        void setPos(const Point2 &pos) { setBox({pos, m_box.size}); }
        void setSize(const Size2 &size) { setBox({m_box.pos, size}); }
        bool setRequiredSize(const Size2 &size) { 
            if(m_requiredSize == size)
                return false;
            m_requiredSize = size; 
            requiresRelayout(); 
            requiresRedraw();
            return true;
        }
        bool setBox(const Rectangle &box) {
            printf("setBox %p {%d,%d,%d,%d}\n", this, box.pos.x, box.pos.y, box.size.w, box.size.h);
            if(m_box == box)
                return false;
            m_box = box; 
            onBoxChange(m_box);
            requiresRelayout();
            requiresRedraw(); 
            return true;
        }

        virtual void redraw(Graphics&, bool force = false) = 0;
        /**
         * Start a re-layout of all children in this object
         * returns false on success
         * returns true if size requirements changed!
        */
        virtual bool relayout(){ 
            printf("default relayout (%p)\n", this);
            return false; 
        };
        void redrawDone() { m_needsRedrawing = false; m_childNeedsRedrawing = false;}
        void relayoutDone() { 
            m_needsRelayout = false; 
            m_childNeedsRelayout = false;
            printf("[DRAW] relayout done (%p)\n", this);
        }

        Drawable *getParent() const { return m_parent; }
        void setParent(Drawable *d) { m_parent = d; }

        bool needsRedrawing() const { return m_needsRedrawing; }
        bool childNeedsRedrawing() const { return m_childNeedsRedrawing; }

        bool needsRelayout() const { return m_needsRelayout; }
        bool childNeedsRelayout() const { return m_childNeedsRelayout; }

    protected:
        void requiresRedraw() {
            m_needsRedrawing = true; 
            if(m_parent)
                m_parent->childRequiresRedraw();
        };
        void requiresRelayout() {
            m_needsRelayout = true; 
            if(m_parent)
                m_parent->childRequiresRelayout();
        };
        void childRequiresRedraw() { 
            m_childNeedsRedrawing = true; 
            if(m_parent)
                m_parent->childRequiresRedraw();
        }
        void childRequiresRelayout() {
            m_childNeedsRelayout = true; 
            if(m_parent)
                m_parent->childRequiresRelayout();
        };

        void clear(gui::Graphics &graphics);
        void clear(gui::Graphics &graphics, gui::Color BG);
        virtual void onBoxChange(const Rectangle&r){
        };

    private:
        bool m_needsRedrawing = true;
        bool m_childNeedsRedrawing = true;
        bool m_needsRelayout = true;
        bool m_childNeedsRelayout = true;

        Drawable* m_parent = nullptr;

        //x,y,w,h
        Rectangle m_box{};
        Size2 m_requiredSize{};
        bool m_grow = false;
        bool m_growH = false, m_growV = false;
        
    };
} // namespace gui
