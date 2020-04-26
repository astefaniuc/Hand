#ifndef HAND_VIEW_DATATYPES_RECT_H
#define HAND_VIEW_DATATYPES_RECT_H

#include <SDL/SDL.h>
#include "graph/data.h"


struct Rel_Rect
{
    Rel_Rect(double x_, double y_, double w_, double h_) : x(x_), y(y_), w(w_), h(h_) {}
    Rel_Rect() : x(0.0), y(0.0), w(1.0), h(1.0) {}
    Rel_Rect(Rel_Rect& c) : x(c.x), y(c.y), w(c.w), h(c.h) {}

    double x = 0.0;
    double y = 0.0;
    double w = 0.0;
    double h = 0.0;
};



class Rect : public Data<Rel_Rect*>
{
public:
    Rect(const std::string& name, Rel_Rect* val);
    ~Rect();

    using Data::set;
    bool set(Rel_Rect* val);
    void reset() override;
};



class RectFactory : public Rect
{
public:
    RectFactory(const std::string& name, double x_, double y_, double w_, double h_)
        : Rect(name, new Rel_Rect(x_, y_, w_, h_))
    {
        type(FACTORY);
    }

    Vertex* _get() override
    {
        return new Rect(name(), new Rel_Rect(Value->x, Value->y, Value->w, Value->h));
    }
};



class Alternate : public Data<Rel_Rect*>
{
public:
    // Alignment parameters are of type "Rect"
    Alternate(Rect* alignment, Rect* alternate_alignment);

    bool execute(Vertex* layout) override;
};



class AlternateFactory : public Vertex
{
public:
    AlternateFactory(const std::string& name, Vertex* rect_factory, Vertex* alt_rect_factory)
        : Vertex(name)
    {
        type(FACTORY);
        attach(rect_factory);
        attach(alt_rect_factory);
    }

    Vertex* _get() override
    {
        return new Alternate(
            dynamic_cast<Rect*>(get(1)->get()), dynamic_cast<Rect*>(get(2)->get()));
    }
};


void Multiply(Rel_Rect* src, Rel_Rect* tgt);
void Multiply(Rel_Rect* src, SDL_Rect* tgt);
// Helper func for extracting a Rel_Rect from a layout
Rel_Rect* GetRect(const std::string& rect_name, Vertex* layout);

#endif // HAND_VIEW_DATATYPES_RECT_H
