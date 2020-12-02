#include "view/layout.h"
#include "view/layers/list.h"


Layouts::Field* Layout::GetField(const std::string& name, bool create)
{
    Layouts::Field* ret = nullptr;
    for (auto field : m_Fields)
    {
        ret = field->GetField(name);
        if (ret)
            return ret;
    }

    if (create)
    {
        ret = new Layouts::Field(name);
        m_Fields.push_back(ret);
    }
    return ret;
}


void Layout::SetField(const std::string& name, Layouts::SAlignment alignment)
{
    GetField(name)->SetAlignment(alignment);
}


bool Layout::IsValid()
{
    for (auto field : m_Fields)
        if (field->IsValid())
            return true;
    return false;
}


unsigned Layout::GetValidFields(std::vector<Layouts::Field*>& out)
{
    for (auto field : m_Fields)
        if (field->IsValid())
            out.push_back(field);
    return out.size();
}


namespace Layouts {


void Align(EAlignment alignment, const SDL_Rect& tgt, SDL_Rect& src)
{
    switch (alignment)
    {
    case Top:
        src.y = tgt.y;
        break;
    case Bottom:
        src.y = tgt.y + tgt.h - src.h;
        break;
    case Left:
        src.x = tgt.x;
        break;
    case Right:
        src.x = tgt.x + tgt.w - src.w;
        break;
    default:
         // Center
        src.x = tgt.x + (tgt.w - src.w) / 2;
        src.y = tgt.y + (tgt.h - src.h) / 2;
    }
}


Field* Field::GetField(const std::string& name) const
{
    if (name == m_Name)
        return (Field*)this;
    if (m_Layout)
        return m_Layout->GetField(name, false);
    return nullptr;
}


void Field::SetLayout(Layout* layout)
{
    delete m_Layout;
    m_Layout = layout;
}


SDL_Rect Field::GetSize(SDL_Rect& outer)
{
    if (m_Layout)
        return m_Layout->GetSize(outer);
    if (m_Layer)
        return m_Layer->UpdateSize(outer);
    return { 0, 0, 0, 0 };
}


SDL_Rect Field::GetAlignedSize(SDL_Rect& outer)
{
    SDL_Rect size = GetSize(outer);
    if ((size.w == 0) || (size.h == 0))
        return size;

    Align(m_Alignment.Parent, outer, size);

    SDL_Rect frame = outer;
    if (m_Alignment.Parent == Top)
        frame.h = size.h;
    else if (m_Alignment.Parent == Bottom)
    {
        frame.y += frame.h - size.h;
        frame.h = size.h;
    }
    else if (m_Alignment.Parent == Left)
        frame.w = size.w;
    else if (m_Alignment.Parent == Right)
    {
        frame.x += frame.w - size.w;
        frame.w = size.w;
    }

    Align(m_Alignment.Field, frame, size);

    frame = GetSize(size);

    if (m_Alignment.Parent == Top)
    {
        outer.h -= frame.h;
        outer.y += frame.h;
    }
    else if (m_Alignment.Parent == Bottom)
        outer.h -= frame.h;
    else if (m_Alignment.Parent == Left)
    {
        outer.w -= frame.w;
        outer.x += frame.w;
    }
    else if (m_Alignment.Parent == Right)
        outer.w -= frame.w;

    return frame;
}


SDL_Rect Field::GetPlacedSize(SDL_Rect& outer)
{
    outer.x += m_Position.x * outer.w;
    outer.y += m_Position.y * outer.h;
    return GetSize(outer);
}

}
