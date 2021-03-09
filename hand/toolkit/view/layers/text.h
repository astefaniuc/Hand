#ifndef HAND_VIEW_LAYER_TEXT_H
#define HAND_VIEW_LAYER_TEXT_H

#include "view/layer.h"
#include "view/theme.h"
#include <string>


namespace Layers {


class Text : public Layer
{
public:
    virtual std::string GetData() = 0;

    void DrawContent(SDL_Surface*) override {}

protected:
    Drawer* GetDrawerFromTheme() override { return GetTheme()->GetTextDrawer(this); }

    // Terminal layer. TODO: remove this from base class.
    void Rebuild() override { SetModified(); }
};


class RawText : public Text
{
public:
    explicit RawText(const std::string& data = std::string()) { SetData(data); }

    void SetData(const std::string& text);
    std::string GetData() override { return m_Text; }

private:
    std::string m_Text;
};


class DataText : public Text
{
public:
    std::string GetData() override;
};

}
#endif // HAND_VIEW_LAYER_TEXT_H
