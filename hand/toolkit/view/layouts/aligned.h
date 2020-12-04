#ifndef HAND_VIEW_LAYOUTS_ALIGNED_H
#define HAND_VIEW_LAYOUTS_ALIGNED_H

#include "view/layout.h"


namespace Layouts { namespace Aligned {


class Map : public Layout
{
public:
    SDL_Rect GetSize(const SDL_Rect& outer) override;
};


class List : public Layouts::List
{
public:
    SDL_Rect GetSize(const SDL_Rect& outer) override;

    EAlignment GetAlignment() { return m_Alignment; }
    void SetAlignment(EAlignment alignment) { m_Alignment = alignment;}

private:
    EAlignment m_Alignment = Center;
};


Map* CreateView();

}}
#endif //HAND_VIEW_LAYOUTS_ALIGNED_H
