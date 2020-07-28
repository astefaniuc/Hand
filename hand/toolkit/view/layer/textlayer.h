#ifndef HAND_VIEW_LAYER_TEXTLAYER_H
#define HAND_VIEW_LAYER_TEXTLAYER_H

#include "layer.h"
#include <string>


class TextLayer : public Layer
{
public:
    Drawer* CreatetDrawer() override;
    void SetData(const std::string& text) { m_Text = text; }

private:
    std::string m_Text;
};

#endif // HAND_VIEW_LAYER_TEXTLAYER_H
