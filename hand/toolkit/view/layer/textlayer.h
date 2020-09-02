#ifndef HAND_VIEW_LAYER_TEXTLAYER_H
#define HAND_VIEW_LAYER_TEXTLAYER_H

#include "layer.h"
#include <string>


class TextLayer : public Layer
{
public:
    explicit TextLayer(const std::string& data = std::string()) { SetData(data); }

    void SetData(const std::string& text) { m_Text = text; }
    const std::string& GetData() { return m_Text; }

protected:
    void Rebuild() override {} // TODO
    Drawer* CreatetDrawer() override;

private:
    std::string m_Text;
};

#endif // HAND_VIEW_LAYER_TEXTLAYER_H
