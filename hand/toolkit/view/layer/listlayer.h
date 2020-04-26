#ifndef HAND_VIEW_LAYER_LISTLAYER_H
#define HAND_VIEW_LAYER_LISTLAYER_H

#include "view/layer/layer.h"


class ListLayer : public Layer
{
public:
    ListLayer(const std::string& name);

    // Set pointer to a data tree node
    void SetContent(Vertex* focus);
    // Requests the input focus
protected:
    void Init() override;
    Vertex* GetLayout(Vertex* data) override;
    bool SetFocus(Vertex*);
    // Returns the list which should be mapped to the InputState
    Vertex* GetControlsList(Vertex* curr_list);
};

#endif // HAND_VIEW_LAYER_LISTLAYER_H
