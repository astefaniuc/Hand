#ifndef HAND_VIEW_LAYER_BUTTONLAYER_H
#define HAND_VIEW_LAYER_BUTTONLAYER_H

#include "view/layer/layer.h"


class DataLayer : public LayerMap
{
public:
    ~DataLayer() { Clear(); }

protected:
    void SetContent(HmiItem* data) override;

    void Rebuild() override;

    void Clear();
    Drawer* CreatetDrawer() override;
    Layout::Node* CreateLayout() override { return Layout::CreateData(); }

    /// Callback.
    void NotifyChanged(HmiItem*) { m_ChangedContent = true; }

    Layer* m_Name = nullptr;
    Layer* m_DataText = nullptr;
    Layer* m_Description = nullptr;
};


class ButtonLayer : public LayerMap
{
public:
    ~ButtonLayer() { Clear(); }

protected:
    void Rebuild() override;

    void Clear();
    Drawer* CreatetDrawer() override;
    Layout::Node* CreateLayout() override { return Layout::CreateButton(); }

    Layer* m_Name = nullptr;
    Layer* m_Description = nullptr;
};


#endif // HAND_VIEW_LAYER_BUTTONLAYER_H
