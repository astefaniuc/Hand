#include "view/datatypes/rect.h"
#include "graph/data.h"


Rect::Rect(
    const std::string& a_name,
    const std::string& a_description,
    double x, double y, double w, double h)
    : Collection(a_name, a_description), m_Value(x, y, w, h)
{
    // For now add plain data directly
    Add(new TData<double>("x", "", m_Value.x));
    Add(new TData<double>("y", "", m_Value.y));
    Add(new TData<double>("w", "", m_Value.w));
    Add(new TData<double>("h", "", m_Value.h));
}


void Rect::SetValue(const Rel_Rect& a_value)
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

// ----------------------------------------------------------------

void Multiply(const Rel_Rect& src, Rel_Rect& tgt)
{
    tgt.x += (src.x * tgt.w);
    tgt.y += (src.y * tgt.h);
    tgt.w *= src.w;
    tgt.h *= src.h;
}


SDL_Rect Multiply(const Rel_Rect& a_rel, const SDL_Rect& a_abs)
{
    SDL_Rect ret;
    ret.x = a_rel.x * a_abs.w;
    ret.y = a_rel.y * a_abs.h;
    ret.w = a_rel.w * a_abs.w;
    ret.h = a_rel.h * a_abs.h;

    return ret;
}
