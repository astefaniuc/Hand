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



void Layout::Align(EAlignment alignment, const SDL_Rect& tgt, SDL_Rect& src) const
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
    default: // Center
        src.x = tgt.x + (tgt.w - src.w) / 2;
        src.y = tgt.y + (tgt.h - src.h) / 2;
    }
}
