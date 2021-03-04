#ifndef HAND_VIEW_LAYER_HMI_DATA_H
#define HAND_VIEW_LAYER_HMI_DATA_H

#include "view/layers/hmi/item.h"
#include "view/layouts/builtin.h"


namespace Layers {


class Data : public Item
{
protected:
    void Rebuild() override;

    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override { return Layouts::CreateData(); }
};

}
#endif // HAND_VIEW_LAYER_HMI_DATA_H
