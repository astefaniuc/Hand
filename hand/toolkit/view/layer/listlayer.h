#ifndef HAND_VIEW_LAYER_LISTLAYER_H
#define HAND_VIEW_LAYER_LISTLAYER_H

#include "view/layer/layer.h"
#include <vector>


class ListLayer : public Layer
{
public:
    ~ListLayer();

    /// Returns 'sub'.
    Layer* Insert(Layer* sub);
    void Remove(Layer* sub) override;

    Layer* GetFirstChild() override;
    Layer* GetNextChild() override;

protected:
    void Rebuild() override;

    Drawer* CreatetDrawer() override;
    Layout::Node* CreateLayout() override { return new Layout::List(); }

    bool SetFocus(HmiItem*);
    // Returns the list which should be mapped to the InputState
    HmiItem* GetControlsList();

    Layout::List* GetListLayout() { return static_cast<Layout::List*>(GetLayout()); }

private:
    std::vector<Layer*> m_Sublayers;
    std::vector<Layer*>::const_iterator m_CurrentChild = m_Sublayers.cbegin();
    unsigned m_StartPosition = 0;
};

#endif // HAND_VIEW_LAYER_LISTLAYER_H
