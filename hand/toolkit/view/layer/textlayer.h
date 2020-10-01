#ifndef HAND_VIEW_LAYER_TEXTLAYER_H
#define HAND_VIEW_LAYER_TEXTLAYER_H

#include "view/layer/layer.h"


class TextLayer : public Layer
{
public:
    explicit TextLayer(const std::string& data = std::string()) { SetData(data); }

    void SetData(const std::string& text) { m_Text = text; }
    const std::string& GetData() { return m_Text; }

protected:
    Drawer* CreatetDrawer() override;

    // Terminal layer. TODO: remove all this from base class.
    void Rebuild() override {}
    void UpdateSubContent() override {}
    void Remove(Layer*) override {}
    Layout::Node* CreateLayout() override { return nullptr; }
    SDL_Rect GetLayoutSize() override { return {1,2,3,4}; }
    void DrawChildren() override {}
    void SetChildrenSizes() override {}

private:
    std::string m_Text;
};

#endif // HAND_VIEW_LAYER_TEXTLAYER_H
