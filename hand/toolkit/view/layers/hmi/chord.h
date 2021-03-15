#ifndef HAND_VIEW_LAYER_HMI_CHORD_H
#define HAND_VIEW_LAYER_HMI_CHORD_H

#include "view/layers/list.h"


namespace Layers {


class Chord : public List
{
protected:
    void Rebuild() override {}
    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override;
};

}
#endif // HAND_VIEW_LAYER_HMI_CHORD_H
