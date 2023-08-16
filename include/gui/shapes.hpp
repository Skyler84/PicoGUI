#pragma once

#include "pico_graphics.hpp"

namespace gui
{
    // struct Point2{
    //     int x, y;
    // };
    using Point2 = pimoroni::Point;
    inline static Point2 operator *(const Point2&p, int s) {
        Point2 p2{p.x*s, p.y*s};
        return p2;
    }
    inline static Point2 &operator *=(Point2&p, int s) {
        p.x *= s;
        p.y *= s;
        return p;
    }
    struct Size2{
        int w,h;
        bool operator == (const Size2 &other){
            if(this->w == other.w && this->h == other.h)
                return true;
            return false;
        }
    };
    struct Shape{
        virtual int getArea() const = 0;
    };
    struct Rectangle : Shape{
        Rectangle():pos{0,0},size{0,0}{}
        Rectangle(int x, int y, int w, int h):pos{x,y},size{w,h}{}
        Rectangle(const Rectangle& rect):pos{rect.pos},size{rect.size}{}
        Rectangle(const Point2& pos):pos{pos},size{0,0}{}
        Rectangle(const Size2& size):pos{0,0},size{size}{}
        Rectangle(const Point2& pos, const Size2& size):pos{pos},size{size}{}
        Point2 pos;
        Size2 size;
        virtual int getArea() const override {return size.w*size.h; }
        operator pimoroni::Rect(){
            return pimoroni::Rect{pos.x, pos.y, size.w, size.h};
        } 
        bool operator == (const Rectangle &other){
            if(this->pos == other.pos && this->size == other.size)
                return true;
            return false;
        }
    };
    struct Circle : Shape{
        int cx, cy, r;
    };


} // namespace gui
