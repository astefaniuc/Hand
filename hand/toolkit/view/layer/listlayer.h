#ifndef HAND_VIEW_LAYER_LISTLAYER_H
#define HAND_VIEW_LAYER_LISTLAYER_H

#include "view/layer/layer.h"


class ListLayer : public Layer
{
protected:
    void Rebuild() override;
    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return new ListLayout(); }

    bool SetFocus(HmiItem*);
    // Returns the list which should be mapped to the InputState
    HmiItem* GetControlsList();

private:
    unsigned m_StartPosition = 0;
};

#endif // HAND_VIEW_LAYER_LISTLAYER_H
