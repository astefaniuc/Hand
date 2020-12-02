#include "view/layouts/placed.h"
#include "view/layers/list.h"


namespace Layouts { namespace Placed {


SDL_Rect Field::GetSize(Layers::List* parent, SDL_Rect& outer)
{
    outer.x += m_Position.x * outer.w;
    outer.y += m_Position.y * outer.h;
    return GetLayerSize(parent, outer);
}



SDL_Rect Map::GetSize(Layers::List* tgt, SDL_Rect& outer)
{
    for (auto field : m_Fields)
    {
        SDL_Rect tmp = outer;
        field->GetSize(tgt, tmp);
    }
    return outer;
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
    Field* field = GetField(name);
    if (!field)
    {
        field = new Field(name);
        m_Fields.push_back(field);
    }
    field->SetPosition(position);
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
