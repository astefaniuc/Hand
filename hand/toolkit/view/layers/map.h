#ifndef HAND_VIEW_LAYER_MAP_H
#define HAND_VIEW_LAYER_MAP_H

#include "view/layers/list.h"


namespace Layers {


class Map : public List
{
protected:
    void Rebuild() override;
};

}
#endif // HAND_VIEW_LAYER_MAP_H
