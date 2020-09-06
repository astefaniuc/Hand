#ifndef HAND_VIEW_LAYER_BUTTONLAYER_H
#define HAND_VIEW_LAYER_BUTTONLAYER_H

#include "view/layer/layer.h"
#include "view/layer/textlayer.h"


class DataLayer : public Layer
{
public:
    ~DataLayer() { Clear(); }

protected:
    void Rebuild() override;
    void Clear();
    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return new DataLayout(); }

    TextLayer* m_Name = nullptr;
    TextLayer* m_DataText = nullptr;
    TextLayer* m_Description = nullptr;
};


class ButtonLayer : public Layer
{
public:
    ~ButtonLayer() { Clear(); }

protected:
    void Rebuild() override;
    void Clear();
    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return new ButtonLayout(); }

    TextLayer* m_Name = nullptr;
    TextLayer* m_Description = nullptr;
};


#endif // HAND_VIEW_LAYER_BUTTONLAYER_H
