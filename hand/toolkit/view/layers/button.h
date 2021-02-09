#ifndef HAND_VIEW_LAYER_BUTTON_H
#define HAND_VIEW_LAYER_BUTTON_H

#include "view/layers/map.h"
#include "view/layouts/builtin.h"


namespace Layers {


class Button : public Map
{
protected:
    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override { return Layouts::CreateButton(); }
};

}
#endif // HAND_VIEW_LAYER_BUTTON_H
