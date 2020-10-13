#include "view/layout.h"
#include "view/layers/map.h"


SDL_Rect Field::GetSize(Layer* a_tgt, SDL_Rect offset)
{
    Layers::Map* tgt = static_cast<Layers::Map*>(a_tgt);
    Layer* sub = tgt->GetField(m_Name);
    if (sub)
        return sub->UpdateSize(offset);
    return { 0, 0, 0, 0 };
}


Field* Field::GetField(const std::string& name) const
{
    if (name == m_Name)
        return (Field*)this;
    return nullptr;
}
