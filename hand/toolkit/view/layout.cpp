#include "view/layout.h"
#include "view/layers/list.h"


SDL_Rect Field::GetLayerSize(Layers::List* parent, SDL_Rect& outer)
{
    Layer* sub = parent->GetChild(m_Name);
    if (sub)
        return sub->UpdateSize(outer);
    return { 0, 0, 0, 0 };
}


Field* Field::GetField(const std::string& name) const
{
    if (name == m_Name)
        return (Field*)this;
    return nullptr;
}
