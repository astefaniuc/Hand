#include "view/datatypes/rect.h"
#include "data/data.h"


SDL_Rect Multiply(const RelRect& a_rel, const SDL_Rect& a_abs)
{
    SDL_Rect ret;
    ret.x = a_rel.x * a_abs.w;
    ret.y = a_rel.y * a_abs.h;
    ret.w = a_rel.w * a_abs.w;
    ret.h = a_rel.h * a_abs.h;

    return ret;
}



Rect::Rect(
    const std::string& a_name,
    const std::string& a_description,
    double x, double y, double w, double h)
    : Data::Vector(a_name, a_description), m_Value(x, y, w, h)
{
    // For now add plain data directly
    Add(new Data::Double("x", "", m_Value.x));
    Add(new Data::Double("y", "", m_Value.y));
    Add(new Data::Double("w", "", m_Value.w));
    Add(new Data::Double("h", "", m_Value.h));
}


void Rect::SetValue(const RelRect& a_value)
{
    m_Value = a_value;
    // For now add plain data directly
    Get("x")->SetValue(m_Value.x);
    Get("y")->SetValue(m_Value.y);
    Get("w")->SetValue(m_Value.w);
    Get("h")->SetValue(m_Value.h);
}


void Rect::Reset()
{
    m_Value.x = Get("x")->GetValue();
    m_Value.y = Get("y")->GetValue();
    m_Value.w = Get("w")->GetValue();
    m_Value.h = Get("h")->GetValue();
}
