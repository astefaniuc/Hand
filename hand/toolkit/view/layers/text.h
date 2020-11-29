#ifndef HAND_VIEW_LAYER_TEXT_H
#define HAND_VIEW_LAYER_TEXT_H

#include "view/layer.h"
#include "view/theme.h"


namespace Layers {


class Text : public Layer
{
public:
    explicit Text(const std::string& data = std::string()) { SetData(data); }

    void SetData(const std::string& text);
    const std::string& GetData() { return m_Text; }

protected:
    Drawer* CreatetDrawer() override { return GetTheme()->GetTextDrawer(); }
    Layout* CreateLayout() override { return new Layout(); }

    // Terminal layer. TODO: remove all this from base class.
    void Rebuild() override {}
    void Remove(Layer*) override {}

private:
    std::string m_Text;
};

}
#endif // HAND_VIEW_LAYER_TEXT_H
