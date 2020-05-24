#ifndef HAND_VIEW_LAYER_LISTLAYER_H
#define HAND_VIEW_LAYER_LISTLAYER_H

#include "view/layer/layer.h"


class ListLayer : public Layer
{
public:
    ListLayer() { BufferType = COLLECTOR; }


    // Set pointer to a data tree node
    void SetContent(HmiItem* focus) override;
    // Requests the input focus
protected:
    void Init() override;
    bool SetFocus(HmiItem*);
    // Returns the list which should be mapped to the InputState
    HmiItem* GetControlsList();
};

#endif // HAND_VIEW_LAYER_LISTLAYER_H
