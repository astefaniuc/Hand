#ifndef HAND_VIEW_LAYOUTS_EXPANDING_H
#define HAND_VIEW_LAYOUTS_EXPANDING_H

#include "view/layout.h"
#include <vector>


namespace Layouts { namespace Placed {

/// Expanding layouts using fixed relative positions for placing their sub-fields.


class Map : public Layout
{
public:
    SDL_Rect GetSize(SDL_Rect& outer) override;

    void SetField(const std::string& name, const RelRect& position) {
        GetField(name)->SetPosition(position);
    }
};


typedef Map List;


Map* CreateView();

}}
#endif //HAND_VIEW_LAYOUTS_EXPANDING_H
