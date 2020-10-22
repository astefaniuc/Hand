#include "view/layouts/placed.h"
#include "view/layer.h"


namespace Layouts { namespace Placed {


SDL_Rect Field::GetSize(Layer* tgt, SDL_Rect outer)
{
    outer.x += m_Position.x * outer.w;
    outer.y += m_Position.y * outer.h;
    return ::Field::GetSize(tgt, outer);
}



SDL_Rect Map::GetSize(Layer* tgt, SDL_Rect outer)
{
    for (auto field : m_Fields)
        field->GetSize(tgt, outer);
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
    Field* field = static_cast<Field*>(GetField(name));
    if (!field)
    {
        field = new Field(name);
        m_Fields.push_back(field);
    }
    field->SetPosition(position);
}



SDL_Rect List::GetSize(Layer* tgt, SDL_Rect outer)
{
    for (auto field : m_Fields)
        field->GetSize(tgt, outer);
    return outer;
}


void List::SetField(const std::string& name, const RelRect& position)
{
    Field* field = static_cast<Field*>(GetField(name));
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
