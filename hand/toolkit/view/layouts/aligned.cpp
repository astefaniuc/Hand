#include "view/layouts/aligned.h"
#include "view/layers/list.h"


namespace Layouts { namespace Aligned {


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



SDL_Rect Field::GetSize(Layers::List* parent, SDL_Rect& outer)
{
    SDL_Rect size = GetLayerSize(parent, outer);
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

    frame = GetLayerSize(parent, size);

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



SDL_Rect Map::GetSize(Layers::List* tgt, SDL_Rect& outer)
{
    SDL_Rect ret = outer;
    for (auto field : m_Fields)
        field->GetSize(tgt, outer);
    return ret;
}


Field* Map::GetField(const std::string& name) const
{
    for (auto field : m_Fields)
        if (field->GetField(name))
            return field;
    return nullptr;
}


void Map::SetField(const std::string& name, SAlignment alignment)
{
    Field* field = GetField(name);
    if (!field)
    {
        field = new Field(name);
        m_Fields.push_back(field);
    }
    field->SetAlignment(alignment);
}


Map* CreateView()
{
    Map* ret = new Map();
    ret->SetField(TITLE, { Top, Center });
    ret->SetField(DESCRIPTION, { Right, Center });
    ret->SetField(MANIPULATORS, { Left, Center });
    ret->SetField(CONTROL, { Bottom, Left });
    ret->SetField(LAYER_CONTROLS, { Bottom, Right });
    ret->SetField(VIEW, { Center, Center });
    return ret;
}



SDL_Rect List::GetSize(Layers::List* tgt, SDL_Rect& outer)
{
    unsigned count = tgt->GetChildCount();
    if (!count)
        return Layout::GetSize(tgt, outer);

    SDL_Rect size = outer;
    SDL_Rect field = outer;

    // Calculate first the fixed size. The list expands only in one direction.
    Layer* sub = tgt->GetFirstChild();
    if (GetOrientation() == Vertical)
    {
        field.h /= count;
        SDL_Rect available = field;
        field.w = 0;
        while (sub)
        {
            SDL_Rect subSize = sub->UpdateSize(available);
            if (subSize.w > field.w)
                field.w = subSize.w;
            sub = tgt->GetNextChild();
        }

        size.w = field.w;
    }
    else
    {
        field.w /= count;
        SDL_Rect available = field;
        field.h = 0;
        while (sub)
        {
            SDL_Rect subSize = sub->UpdateSize(available);
            if (subSize.h > field.h)
                field.h = subSize.h;
            sub = tgt->GetNextChild();
        }

        size.h = field.h;
    }

    // Reset iterator.
    sub = tgt->GetFirstChild();
    // Align each sub-layer into its field.
    while (sub)
    {
        SDL_Rect subSize = sub->UpdateSize(field);
        Align(m_Alignment, field, subSize);
        sub->UpdateSize(subSize);

        if (GetOrientation() == Vertical)
            field.y += field.h;
        else
            field.x += field.w;
        sub = tgt->GetNextChild();
    }

    return size;
}

}}
