#ifndef HAND_VIEW_LAYER_BUTTONLAYER_H
#define HAND_VIEW_LAYER_BUTTONLAYER_H

#include "view/layer/layer.h"
#include "view/layer/textlayer.h"


class ButtonLayer : public Layer
{
public:
    ButtonLayer();

protected:
    void Rebuild() override;
    Drawer* CreatetDrawer() override;

    TextLayer* m_Name = nullptr;
    TextLayer* m_Description = nullptr;
    TextLayer* m_Command = nullptr;
};

#endif // HAND_VIEW_LAYER_BUTTONLAYER_H
