#ifndef HAND_VIEW_LAYER_TEXT_H
#define HAND_VIEW_LAYER_TEXT_H

#include "view/layer.h"
#include "view/theme.h"
#include <string>


namespace Layers {


class Text : public Layer
{
public:
    explicit Text(const std::string& data = std::string()) { SetData(data); }

    void SetData(const std::string& text);
    const std::string& GetData() { return m_Text; }

    void DrawContent(SDL_Surface*) override {}

protected:
    Drawer* CreatetDrawer() override { return GetTheme()->GetTextDrawer(this); }

    // Terminal layer. TODO: remove this from base class.
    void Rebuild() override {}

private:
    std::string m_Text;
};

}
#endif // HAND_VIEW_LAYER_TEXT_H
