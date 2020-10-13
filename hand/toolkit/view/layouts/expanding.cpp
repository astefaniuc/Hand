#include "view/layouts/expanding.h"
#include "view/layer.h"


namespace Layouts { namespace Expanding {


SDL_Rect Field::GetSize(Layer* tgt, SDL_Rect offset)
{
    offset.x += m_Position.x * offset.w;
    offset.y += m_Position.y * offset.h;
    return ::Field::GetSize(tgt, offset);
}



SDL_Rect Map::GetSize(Layer* tgt, SDL_Rect offset)
{
    for (auto field : m_Fields)
        field->GetSize(tgt, offset);
    return offset;
}


Field* Map::GetField(const std::string& name) const
{
    for (auto field : m_Fields)
        if (field->GetField(name))
            return field;
    return nullptr;
}


void Map::SetField(const std::string& name, const RelRect& position)
{
    Field* field = static_cast<Field*>(GetField(name));
    if (!field)
    {
        field = new Field(name);
        m_Fields.push_back(field);
    }
    field->SetPosition(position);
}



SDL_Rect List::GetSize(Layer* tgt, SDL_Rect offset)
{
    unsigned count = 0;

    Layer* sub = tgt->GetFirstChild();
    while (sub)
    {
        ++count;
        sub = tgt->GetNextChild();
    }
    if (!count)
        return Layout::GetSize(tgt, offset);
    if (count > GetMaxItemsToShow())
        count = GetMaxItemsToShow();

    SDL_Rect size = offset;
    if (GetOrientation() == Vertical)
    {
        offset.h /= count;
        size.w = 0;
        while (sub)
        {
            SDL_Rect subSize = sub->UpdateSize(offset);
            if (subSize.w > size.w)
                size.w = subSize.w;
            offset.y += offset.h;

            sub = tgt->GetNextChild();
        }
    }
    else
    {
        offset.w /= count;
        size.h = 0;
        while (sub)
        {
            SDL_Rect subSize = sub->UpdateSize(offset);
            if (subSize.h > size.h)
                size.h = subSize.h;
            offset.x += offset.w;
            sub = tgt->GetNextChild();
        }
    }

    return size;
}


Map* CreateButton()
{
    Map* ret = new Map();
    ret->SetField(CONTROL, { 0.8, 0.6, 0.2, 0.4 });
    ret->SetField(DESCRIPTION, { 0.0, 0.6, 0.8, 0.4 });
    ret->SetField(TITLE, { 0.25, 0.0, 0.75, 0.6 });
    ret->SetField(VIEW, { 0.0, 0.0, 0.25, 0.6 });
    return ret;
}


Map* CreateData()
{
    Map* ret = new Map();
    ret->SetField(CONTROL, { 0.8, 0.6, 0.2, 0.4 });
    ret->SetField(DESCRIPTION, { 0.0, 0.6, 0.8, 0.4 });
    ret->SetField(TITLE, { 0.0, 0.0, 0.25, 0.6 });
    ret->SetField(VIEW, { 0.25, 0.0, 0.75, 0.6 });
    return ret;
}


Map* CreateView()
{
    Map* ret = new Map();
    ret->SetField(CONTROL, { 0.2, 0.8, 0.6, 0.2 });
    ret->SetField(DESCRIPTION, { 0.8, 0.3, 0.2, 0.4 });
    ret->SetField(TITLE, { 0.0, 0.0, 1.0, 0.04 });
    ret->SetField(VIEW, { 0.0, 0.04, 0.8, 0.76 });
    return ret;
}

}}
