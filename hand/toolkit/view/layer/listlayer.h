#ifndef HAND_VIEW_LAYER_LISTLAYER_H
#define HAND_VIEW_LAYER_LISTLAYER_H

#include "view/layer/layer.h"
#include <vector>


class ListLayer : public Layer
{
public:
    ~ListLayer();

    void DrawChildren() override;
    /// Returns 'sub'.
    Layer* Insert(Layer* sub);
    void Remove(Layer* sub) override;
    void SetChildrenSizes() override;

    const std::vector<Layer*>& GetSubLayers() const { return m_Sublayers; }

protected:
    void Rebuild() override;
    void UpdateSubContent() override;

    SDL_Rect GetLayoutSize() override {
        return GetListLayout()->GetFieldSize(this, { 0, 0, 0, 0 });
    }

    Drawer* CreatetDrawer() override;
    Layout::Node* CreateLayout() override { return new Layout::List(); }

    bool SetFocus(HmiItem*);
    // Returns the list which should be mapped to the InputState
    HmiItem* GetControlsList();

    Layout::List* GetListLayout() { return static_cast<Layout::List*>(GetLayout()); }

private:
    std::vector<Layer*> m_Sublayers;
    unsigned m_StartPosition = 0;
};

#endif // HAND_VIEW_LAYER_LISTLAYER_H
