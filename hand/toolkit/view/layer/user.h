#ifndef HAND_VIEW_LAYER_LAYERMANAGER_H
#define HAND_VIEW_LAYER_LAYERMANAGER_H

#include "view/layer/listlayer.h"


class Device;
class InputState;


/// Manages the items to be shown on screen and the input modes.
class CUser : public Layer
{
public:
    CUser();
    virtual ~CUser();

    bool Update(bool force) override;

    bool GetCommand(HmiItem* func, int level);
    void SetDevice(Device* device);

    // Stores the pointer to the object to be displayed next
    bool Request(HmiItem* request);

protected:
    void Init() override;

private:
    bool Expand(HmiItem* list);
    bool Exit(HmiItem* ignore);

    // Contains the only pointer to the Device
    Device* _Device = nullptr;
    HmiItem* NextRequest = nullptr;
    Layer* MainView = nullptr;
    HmiItem m_UI;
};

#endif // HAND_VIEW_LAYER_LAYERMANAGER_H
