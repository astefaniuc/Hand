#ifndef HAND_VIEW_LAYER_BUTTON_H
#define HAND_VIEW_LAYER_BUTTON_H

#include "view/layers/map.h"
#include "view/layouts/compact.h"


namespace Layers {


class Button : public Map
{
protected:
    void Rebuild() override;

    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return Layouts::Compact::CreateButton(); }
};

}
#endif // HAND_VIEW_LAYER_BUTTON_H
