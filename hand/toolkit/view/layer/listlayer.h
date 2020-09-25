#ifndef HAND_VIEW_LAYER_LISTLAYER_H
#define HAND_VIEW_LAYER_LISTLAYER_H

#include "view/layer/layer.h"
#include <vector>


class ListLayer : public Layer
{
public:
    ~ListLayer();

    void DrawChildren(bool forced) override;
    /// Returns 'sub'.
    Layer* Insert(Layer* sub);
    void Remove(Layer* sub) override;

    SDL_Rect GetLayoutSize(const SDL_Rect& offset) override {
        return GetListLayout()->GetFieldSize(this, GetDrawer()->GetFrameOffset());
    }
    const std::vector<Layer*>& GetSubLayers() const { return m_Sublayers; }

protected:
    void Rebuild() override;
    void UpdateSubContent() override;

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
