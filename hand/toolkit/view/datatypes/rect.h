#ifndef HAND_VIEW_DATATYPES_RECT_H
#define HAND_VIEW_DATATYPES_RECT_H

#include <SDL2/SDL.h>
#include "data/vector.h"
#include "data/data.h"


struct RelRect
{
    RelRect(double a_x = 0.0, double a_y = 0.0, double a_w = 1.0, double a_h = 1.0)
        : x(a_x), y(a_y), w(a_w), h(a_h) {}

    double x;
    double y;
    double w;
    double h;
};



class Rect : public Data::Vector
{
public:
    Rect(
        const std::string& name,
        const std::string& description,
        double x, double y, double w, double h);

    void SetValue(const RelRect& value);
    const RelRect& GetValue() { return m_Value; }
    void Reset();

private:
    Data::Double* Get(const std::string& coordinate)
    {
        return (Data::Double*)GetChild(coordinate);
    }

    RelRect m_Value;
};


SDL_Rect Multiply(const RelRect& a_clipping, const SDL_Rect& a_absSrc);

#endif // HAND_VIEW_DATATYPES_RECT_H
