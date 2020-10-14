#include "view/layouts/aligned.h"
#include "view/layer.h"


namespace Layouts { namespace Aligned {


SDL_Rect Field::GetSize(Layer* tgt, SDL_Rect offset)
{
    SDL_Rect size = ::Field::GetSize(tgt, offset);

    Align(m_Alignment.Parent, offset, size);

    if (m_Alignment.Parent == Top)
        offset.h = size.h;
    else if (m_Alignment.Parent == Bottom)
    {
        offset.y += offset.h - size.h;
        offset.h = size.h;
    }
    else if (m_Alignment.Parent == Left)
        offset.w = size.w;
    else if (m_Alignment.Parent == Right)
    {
        offset.x += offset.w - size.w;
        offset.w = size.w;
    }

    Align(m_Alignment.Field, offset, size);

    return ::Field::GetSize(tgt, size);
}



SDL_Rect Map::GetSize(Layer* tgt, SDL_Rect offset)
{
    SDL_Rect ret = offset;
    for (auto field : m_Fields)
    {
        SDL_Rect sub = field->GetSize(tgt, offset);
        SAlignment align = field->GetAlignment();
        if (align.Parent == Top)
        {
            offset.h -= sub.h;
            offset.y += sub.h;
        }
        else if (align.Parent == Bottom)
            offset.h -= sub.h;
        else if (align.Parent == Left)
        {
            offset.w -= sub.w;
            offset.x += sub.w;
        }
        else if (align.Parent == Right)
            offset.w -= sub.w;
    }
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
    Field* field = static_cast<Field*>(GetField(name));
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
    ret->SetField(TITLE, { Layout::Top, Layout::Center });
    ret->SetField(CONTROL, { Layout::Bottom, Layout::Center });
    ret->SetField(DESCRIPTION, { Layout::Right, Layout::Center });
    ret->SetField(VIEW, { Layout::Center, Layout::Center });
    return ret;
}

}}
