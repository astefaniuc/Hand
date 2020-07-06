#ifndef HAND_VIEW_LAYER_USER_H
#define HAND_VIEW_LAYER_USER_H

#include "graph/interface.h"
#include "view/layer/layer.h"

class Device;

/// Manages the items to be shown on screen and the input modes.
class CUser : public Layer
{
public:
    CUser();
    virtual ~CUser();

    bool GetCommand(HmiItem* func, int level);
    void SetDevice(Device* device);

private:
    // Contains the only pointer to the Device
    Device* _Device = nullptr;
    Interface m_UI;
};

#endif // HAND_VIEW_LAYER_USER_H
