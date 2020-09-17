#ifndef HAND_VIEW_LAYER_BUTTONLAYER_H
#define HAND_VIEW_LAYER_BUTTONLAYER_H

#include "view/layer/layer.h"


class DataLayer : public Layer
{
public:
    ~DataLayer() { Clear(); }

protected:
    void Rebuild() override;
    void UpdateSubSizes() override;

    void Clear();
    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return new DataLayout(); }

    Layer* m_Name = nullptr;
    Layer* m_DataText = nullptr;
    Layer* m_Description = nullptr;
};


class ButtonLayer : public Layer
{
public:
    ~ButtonLayer() { Clear(); }

protected:
    void Rebuild() override;
    void UpdateSubSizes() override;

    void Clear();
    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return new ButtonLayout(); }

    Layer* m_Name = nullptr;
    Layer* m_Description = nullptr;
};


#endif // HAND_VIEW_LAYER_BUTTONLAYER_H
