#include "view/field.h"
#include "view/layers/list.h"


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


SDL_Rect Field::GetSize(const SDL_Rect& outer)
{
    if (m_Layout)
        return m_Layout->GetSize(outer);
    if (m_Layer)
        return m_Layer->UpdateSize(outer);
    return { 0, 0, 0, 0 };
}


void Field::Align()
{
    m_AlignmentV.Align(Frame, Size);
    m_AlignmentH.Align(Frame, Size);

    if (m_Layout)
        m_Layout->UpdatePositions(Size);
    if (m_Layer)
        m_Layer->UpdatePositions(Size);
}


void Field::SetPlacedPosition(const SDL_Rect& outer)
{
    Size.x = outer.x + m_Position.x * outer.w;
    Size.y = outer.y + m_Position.y * outer.h;
    if (m_Layout)
        m_Layout->UpdatePositions(Size);
    if (m_Layer)
        m_Layer->UpdatePositions(Size);
}


void Field::SetExpanding(bool vertical, bool horizontal)
{
    m_ExpandV = vertical;
    m_ExpandH = horizontal;
}


bool Field::IsExpanding(Layout::Orientation direction)
{
    bool ret = (direction == Layout::Vertical) ? m_ExpandV : m_ExpandH;
    if (ret)
        return true;

    if (m_Layout)
        return m_Layout->IsExpanding(direction);
    if (m_Layer)
        return m_Layer->IsExpanding(direction);

    return false;
}


void Field::SetAlignment(VAlignment::Position vertical, HAlignment::Position horizontal)
{
    m_AlignmentV.Pos = vertical;
    m_AlignmentH.Pos = horizontal;
}
