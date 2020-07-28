#ifndef HAND_VIEW_LAYER_BUTTONLAYER_H
#define HAND_VIEW_LAYER_BUTTONLAYER_H

#include "view/layer/layer.h"
#include "view/layer/textlayer.h"


class ButtonLayer : public Layer
{
public:
    ButtonLayer();

    void SetContent(HmiItem* data) override;
    Drawer* CreatetDrawer() override;

protected:
    TextLayer* m_Name = nullptr;
    TextLayer* m_Description = nullptr;
    TextLayer* m_Command = nullptr;
};

#endif // HAND_VIEW_LAYER_BUTTONLAYER_H
