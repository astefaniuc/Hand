#ifndef HAND_VIEW_LAYER_HMI_DATA_H
#define HAND_VIEW_LAYER_HMI_DATA_H

#include "view/layers/list.h"
#include "view/layouts/builtin.h"


namespace Layers {


class Data : public List
{
protected:
    void Rebuild() override {}
    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override { return new Layouts::List(); }
};

}
#endif // HAND_VIEW_LAYER_HMI_DATA_H
