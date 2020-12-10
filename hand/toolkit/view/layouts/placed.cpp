#include "view/layouts/placed.h"
#include "view/layers/list.h"


namespace Layouts { namespace Placed {



SDL_Rect Map::ComputeSize(const SDL_Rect& outer)
{
    std::vector<Field*> fields;
    GetValidFields(fields);
    for (auto field : fields)
        field->ComputeSize(outer);
    return outer;
}


void Map::UpdatePositions(const SDL_Rect& outer)
{
    std::vector<Field*> all;
    GetValidFields(all);
    for (auto field : all)
        field->SetPlacedPosition(outer);
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
