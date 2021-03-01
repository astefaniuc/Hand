#ifndef HAND_VIEW_LAYER_HMI_BUTTON_H
#define HAND_VIEW_LAYER_HMI_BUTTON_H

#include "view/layers/hmi/item.h"
#include "view/layouts/builtin.h"


namespace Layers {


class Button : public Item
{
protected:
    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override { return Layouts::CreateButton(); }
};

}
#endif // HAND_VIEW_LAYER_HMI_BUTTON_H
