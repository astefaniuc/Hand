#include "view/layout.h"
#include "view/field.h"
#include "view/layers/list.h"


Layout::~Layout()
{
    for (auto field : m_Fields)
        delete field;
}


Field* Layout::GetField(const std::string& name, bool create)
{
    Field* ret = nullptr;
    for (auto field : m_Fields)
    {
        ret = field->GetField(name);
        if (ret)
            return ret;
    }

    if (create)
    {
        ret = new Field(name);
        m_Fields.push_back(ret);
    }
    return ret;
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


unsigned Layout::GetValidFields(std::vector<Field*>& out)
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


Field* MakeField(Layout* in)
{
    Field* ret = new Field("");
    ret->SetLayout(in);
    return ret;
}
Field* MakeField(const std::string& in) { return new Field(in); }

Layout* Split(Field* field1, Field* field2, Layout::Orientation orientation)
{
    List* ret = new List();
    ret->SetField(field1);
    ret->SetField(field2);
    ret->SetOrientation(orientation);
    return ret;
}



SDL_Rect List::ComputeSize(const SDL_Rect& outer)
{
    std::vector<Field*> all;
    if (!GetValidFields(all))
        return { 0, 0, 0, 0 };

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
        field->Frame = field->ComputeSize(outer);

    if (expanding.size())
    {
        SDL_Rect rest = outer;
        if (compact.size())
        {
            SDL_Rect compactSize = GetCompoundSize(compact);
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

    SetSameSize(all, GetOrientation());

    return GetCompoundSize(all);
}


void List::UpdatePositions(const SDL_Rect& outer)
{
    std::vector<Field*> all;
    GetValidFields(all);
    unsigned offset = 0;
    for (auto field : all)
    {
        field->Frame.x = outer.x;
        field->Frame.y = outer.y;

        if (GetOrientation() == Vertical)
        {
            field->Frame.y += offset;
            field->Align();
            offset += field->Frame.h;
        }
        else
        {
            field->Frame.x += offset;
            field->Align();
            offset += field->Frame.w;
        }
    }
}


SDL_Rect List::GetCompoundSize(const std::vector<Field*>& fields)
{
    SDL_Rect size = { 0, 0, 0, 0 };

    for (auto field : fields)
    {
        if (GetOrientation() == Vertical)
        {
            size.h += field->Frame.h;
            if (field->Frame.w > size.w)
                size.w = field->Frame.w;
        }
        else
        {
            size.w += field->Frame.w;
            if (field->Frame.h > size.h)
                size.h = field->Frame.h;
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
            if (field->Frame.w > fixedSize)
                fixedSize = field->Frame.w;
        }
        else if (field->Frame.h > fixedSize)
            fixedSize = field->Frame.h;
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
        field->ComputeSize(outer);
        field->Frame = outer;
    }
}

}
