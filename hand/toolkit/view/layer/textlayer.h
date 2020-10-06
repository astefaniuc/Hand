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
    Layout::Node* CreateLayout() override { return new Layout::Node(); }

    // Terminal layer. TODO: remove all this from base class.
    void Rebuild() override {}
    void Remove(Layer*) override {}

private:
    std::string m_Text;
};

#endif // HAND_VIEW_LAYER_TEXTLAYER_H
