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


void Layout::SetField(
    const std::string& name, VAlignment::Position vertical, HAlignment::Position horizontal)
{
    GetField(name)->SetAlignment(vertical, horizontal);
}


bool Layout::IsValid()
{
    for (auto field : m_Fields)
        if (field->IsValid())
            return true;
    return false;
}


bool Layout::IsExpanding(Orientation direction)
{
    for (auto field : m_Fields)
    {
        if (!field->IsValid())
            continue;

        if (field->IsExpanding(direction))
            return true;
    }
    return false;
}


unsigned Layout::GetValidFields(std::vector<Layouts::Field*>& out)
{
    for (auto field : m_Fields)
        if (field->IsValid())
            out.push_back(field);
    return out.size();
}


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


namespace Layouts {


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
    GetSize(Size);
}


SDL_Rect Field::GetPlacedSize(const SDL_Rect& outer)
{
    SDL_Rect tmp = outer;
    tmp.x += m_Position.x * tmp.w;
    tmp.y += m_Position.y * tmp.h;
    return GetSize(tmp);
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

    return false;
}


void Field::SetAlignment(VAlignment::Position vertical, HAlignment::Position horizontal)
{
    m_AlignmentV.Pos = vertical;
    m_AlignmentH.Pos = horizontal;
}


SDL_Rect List::GetSize(const SDL_Rect& outer)
{
    std::vector<Field*> all;
    if (!GetValidFields(all))
        return Layout::GetSize(outer);

    std::vector<Field*> expanding;
    std::vector<Field*> compact;
    for (auto field : all)
    {
        if (field->IsExpanding(GetOrientation()))
            expanding.push_back(field);
        else
            compact.push_back(field);
    }

    for (auto field : compact)
    {
        field->Size = field->GetSize(outer);
        field->Frame = { outer.x, outer.y, field->Size.w, field->Size.h };
    }

    if (expanding.size())
    {
        SDL_Rect rest = outer;
        if (compact.size())
        {
            SDL_Rect compactSize = GetCompactSize(compact);
            // TODO: deal with size.h > outer.h ?
            if (GetOrientation() == Vertical)
            {
                rest.h -= compactSize.h;
                rest.h /= expanding.size();
            }
            else
            {
                rest.w -= compactSize.w;
                rest.w /= expanding.size();
            }
        }

        SetExpandedSize(rest, expanding);
    }
    else
    {
        if (m_ExpansionMode == EqualSize)
            SetEqualSize(compact);
    }

    SDL_Rect size = { outer.x, outer.y, 0, 0 };
    uint16_t fixedSize = SetSameSize(all, GetOrientation());
    if (GetOrientation() == Vertical)
        size.w = fixedSize;
    else
        size.h = fixedSize;

    for (auto field : all)
    {
        if (GetOrientation() == Vertical)
        {
            field->Frame.y += size.h;
            field->Align();
            size.h += field->Frame.h;
        }
        else
        {
            field->Frame.x += size.w;
            field->Align();
            size.w += field->Frame.w;
        }
    }

    return size;
}

uint16_t List::SetSameSize(const std::vector<Field*>& fields, Layout::Orientation orientation)
{
    uint16_t fixedSize = 0;
    for (auto field : fields)
    {
         if (orientation == Vertical)
        {
            if (field->Size.w > fixedSize)
                fixedSize = field->Size.w;
        }
        else if (field->Size.h > fixedSize)
            fixedSize = field->Size.h;
    }

    for (auto field : fields)
    {
        if (orientation == Vertical)
            field->Frame.w = fixedSize;
        else
            field->Frame.h = fixedSize;
    }

    return fixedSize;
}

void List::SetEqualSize(const std::vector<Field*>& fields)
{
    Layout::Orientation orientation = Vertical;
    if (GetOrientation() == Vertical)
        // Inverted
        orientation = Horizontal;

    SetSameSize(fields, orientation);
}


void List::SetExpandedSize(SDL_Rect outer, const std::vector<Field*>& fields)
{
    for (auto field : fields)
    {
        field->Size = field->GetSize(outer);
        field->Frame = outer;
    }
}


SDL_Rect List::GetCompactSize(const std::vector<Field*>& fields)
{
    SDL_Rect size = { 0, 0, 0, 0 };

    for (auto field : fields)
    {
        if (GetOrientation() == Vertical)
        {
            size.h += field->Size.h;
            if (field->Size.w > size.w)
                size.w = field->Size.w;
        }
        else
        {
            size.w += field->Size.w;
            if (field->Size.h > size.h)
                size.h = field->Size.h;
        }
    }

    return size;
}

}
