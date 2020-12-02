#ifndef HAND_VIEW_LAYOUTS_EXPANDING_H
#define HAND_VIEW_LAYOUTS_EXPANDING_H

#include "view/layout.h"
#include <vector>


namespace Layouts { namespace Placed {

/// Expanding layouts using fixed relative positions for placing their sub-fields.

class Field : public ::Field
{
public:
    Field(const std::string& name) : ::Field(name) {}

    void SetPosition(const RelRect& pos) { m_Position = pos;}
    SDL_Rect GetSize(Layers::List* tgt, SDL_Rect& outer) override;

private:
    RelRect m_Position;
};


class Map : public Layout
{
public:
    SDL_Rect GetSize(Layers::List* tgt, SDL_Rect& outer) override;

    Field* GetField(const std::string& name) const override;
    void SetField(const std::string& name, const RelRect& coordinates);

private:
    std::vector<Field*> m_Fields;
};


typedef Map List;


Map* CreateView();

}}
#endif //HAND_VIEW_LAYOUTS_EXPANDING_H
