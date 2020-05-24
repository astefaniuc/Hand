#include "view/datatypes/rect.h"
#include "graph/data.h"


Rect::Rect(
    const std::string& a_name,
    const std::string& a_description,
    const Rel_Rect& a_value,
    Module* a_manipulator) : Collection(a_name, a_description, a_manipulator), m_Value(a_value)
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

void Multiply(Rel_Rect* src, Rel_Rect* tgt)
{
    if(!src || !tgt)
        return;
    tgt->x += (src->x * tgt->w);
    tgt->y += (src->y * tgt->h);
    tgt->w *= src->w;
    tgt->h *= src->h;
}


void Multiply(Rel_Rect* src, SDL_Rect* tgt)
{
    if(!src || !tgt)
        return;
    tgt->x += (src->x * tgt->w);
    tgt->y += (src->y * tgt->h);
    tgt->w *= src->w;
    tgt->h *= src->h;
}
