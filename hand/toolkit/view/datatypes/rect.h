#ifndef HAND_VIEW_DATATYPES_RECT_H
#define HAND_VIEW_DATATYPES_RECT_H

#include <SDL/SDL.h>
#include "graph/collection.h"
#include "graph/data.h"


struct Rel_Rect
{
    Rel_Rect(double x_, double y_, double w_, double h_) : x(x_), y(y_), w(w_), h(h_) {}
    Rel_Rect() : x(0.0), y(0.0), w(1.0), h(1.0) {}
    Rel_Rect(const Rel_Rect& c) : x(c.x), y(c.y), w(c.w), h(c.h) {}

    double x = 0.0;
    double y = 0.0;
    double w = 0.0;
    double h = 0.0;
};



class Rect : public Collection
{
public:
    Rect(
        const std::string& name,
        const std::string& description,
        const Rel_Rect& value,
        Module* manipulator = nullptr);

    void SetValue(const Rel_Rect& value);
    const Rel_Rect& GetValue() { return m_Value; }
    void Reset();

private:
    TData<double>* Get(const std::string& coordinate)
    {
        return (TData<double>*)GetChild(coordinate);
    }

    Rel_Rect m_Value;
};


void Multiply(const Rel_Rect& src, Rel_Rect& tgt);
void Multiply(const Rel_Rect& src, SDL_Rect& tgt);

#endif // HAND_VIEW_DATATYPES_RECT_H
