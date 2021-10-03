#ifndef HAND_VIEW_LAYER_HMI_DATA_H
#define HAND_VIEW_LAYER_HMI_DATA_H

#include "view/layers/list.h"


namespace Layers
{

class Data : public List
{
protected:
    void Rebuild() override {}
    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override;
};

}
#endif // HAND_VIEW_LAYER_HMI_DATA_H
