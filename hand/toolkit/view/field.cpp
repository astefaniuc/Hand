#include "view/field.h"
#include "view/layout.h"


void VAlignment::Align(const SDL_Rect& tgt, SDL_Rect& src)
{
    switch (Pos)
    {
    case Top:
        src.y = tgt.y;
        break;
    case Bottom:
        src.y = tgt.y + tgt.h - src.h;
        break;
    default: // VCenter
        src.y = tgt.y + (tgt.h - src.h) / 2;
    }
}


void HAlignment::Align(const SDL_Rect& tgt, SDL_Rect& src)
{
    switch (Pos)
    {
    case Left:
        src.x = tgt.x;
        break;
    case Right:
        src.x = tgt.x + tgt.w - src.w;
        break;
    default: // HCenter
        src.x = tgt.x + (tgt.w - src.w) / 2;
    }
}



void Field::Item::SetParentField(Field* field)
{
    if (m_Field && (m_Field != field))
        m_Field->RemoveItem();

    m_Field = field;
}


Layers::List* Field::Item::GetParentLayer()
{
    if (GetParentFieldItem())
        return GetParentFieldItem()->GetListLayer();
    return nullptr;
}


Field::Item* Field::Item::GetParentFieldItem()
{
    if (m_Field)
        return m_Field->GetParentLayout();
    return nullptr;
}



void Field::Item::SetModified(bool state)
{
    m_IsModified = state;
    if (!state)
        return;

    Field::Item* parent = GetParentFieldItem();
    if (parent && !parent->IsModified())
        parent->SetModified();
    m_IsModified = true;
}


Field::~Field()
{
    if (m_Item)
        delete m_Item;
}


Field* Field::GetField(const std::string& name) const
{
    if (name == m_Name)
        return (Field*)this;
    if (m_Item)
        return m_Item->GetField(name, false);
    return nullptr;
}


void Field::SetItem(Item* item)
{
    if (m_Item && (item != m_Item))
        delete m_Item;
    m_Item = item;
    m_Item->SetParentField(this);
    m_Item->SetModified();
}


void Field::RemoveItem()
{
    if (!m_Item)
        return;
    m_Item = nullptr;
    m_Size = { 0, 0, 0, 0 };

    if (m_Parent)
        m_Parent->SetModified();
}


SDL_Rect Field::ComputeSize(const SDL_Rect& outer)
{
    m_Size = { 0, 0, 0, 0 };
    if (m_Item)
        m_Size = m_Item->ComputeSize(outer);
    return m_Size;
}


void Field::Align()
{
    m_AlignmentV.Align(Frame, m_Size);
    m_AlignmentH.Align(Frame, m_Size);

    if (m_Item)
        m_Item->UpdatePositions(m_Size);
}


void Field::SetPlacedPosition(const SDL_Rect& outer)
{
    m_Size.x = outer.x + m_Position.x * outer.w;
    m_Size.y = outer.y + m_Position.y * outer.h;

    if (m_Item)
        m_Item->UpdatePositions(m_Size);
}


void Field::SetExpanding(bool vertical, bool horizontal)
{
    m_ExpandV = vertical;
    m_ExpandH = horizontal;
}


bool Field::IsExpanding(Orientation direction) const
{
    bool ret = (direction == Vertical) ? m_ExpandV : m_ExpandH;
    if (ret)
        return true;

    if (m_Item)
        return m_Item->IsExpanding(direction);

    return false;
}


void Field::SetAlignment(VAlignment::Position vertical, HAlignment::Position horizontal)
{
    m_AlignmentV.Pos = vertical;
    m_AlignmentH.Pos = horizontal;
}
