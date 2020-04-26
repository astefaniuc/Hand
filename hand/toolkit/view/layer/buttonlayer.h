#ifndef HAND_VIEW_LAYER_BUTTONLAYER_H
#define HAND_VIEW_LAYER_BUTTONLAYER_H

#include "view/layer/layer.h"


class ButtonLayer : public Layer
{
public:
    ButtonLayer(const std::string& name);
    ~ButtonLayer();

    void SetContent(Vertex* data) override;

private:
    void SetExpandable();
    bool ExpandList(Vertex*);

    Vertex* FunctionBox = nullptr;
};

#endif // HAND_VIEW_LAYER_BUTTONLAYER_H
