#include "view/layers/map.h"


namespace Layers {


void Map::Rebuild()
{
   // TODO
}


void Map::Insert(const std::string& name, Layer* child)
{
    if (!child)
        return;

    Field* field = GetLayout()->GetField(name, false);
    if (!field)
        return;

    field->SetItem(child);
    SetModified();
}

}
