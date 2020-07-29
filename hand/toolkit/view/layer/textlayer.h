#ifndef HAND_VIEW_LAYER_TEXTLAYER_H
#define HAND_VIEW_LAYER_TEXTLAYER_H

#include "layer.h"
#include <string>


class TextLayer : public Layer
{
public:
    void SetData(const std::string& text) { m_Text = text; }

protected:
    void Update() override {} // TODO
    Drawer* CreatetDrawer() override;

private:
    std::string m_Text;
};

#endif // HAND_VIEW_LAYER_TEXTLAYER_H
