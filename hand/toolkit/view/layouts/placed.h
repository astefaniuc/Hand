#ifndef HAND_VIEW_LAYOUTS_EXPANDING_H
#define HAND_VIEW_LAYOUTS_EXPANDING_H

#include "view/layout.h"
#include "view/field.h"


namespace Layouts { namespace Placed {

/// Expanding layouts using fixed relative positions for placing their sub-fields.


class Map : public Layout
{
public:
    SDL_Rect ComputeSize(const SDL_Rect& outer) override;
    void UpdatePositions(const SDL_Rect& outer) override;

    void SetField(const std::string& name, const RelRect& position) {
        GetField(name)->SetPosition(position);
    }

    bool IsExpanding(Orientation) override { return true; }
};


Map* CreateView();

}}
#endif //HAND_VIEW_LAYOUTS_EXPANDING_H
