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
};


Map* CreateView();

}}
#endif //HAND_VIEW_LAYOUTS_ALIGNED_H
