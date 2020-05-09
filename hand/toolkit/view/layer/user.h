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

    bool GetCommand(Vertex* func, int level);
    void SetDevice(Device* device);

    // Stores the pointer to the object to be displayed next
    bool Request(Vertex* request);

protected:
    void Init() override;

private:
    bool Expand(Vertex* list);
    bool Exit(Vertex* ignore);

    // Contains the only pointer to the Device
    Device* _Device = nullptr;
    Vertex* NextRequest = nullptr;
    Layer* MainView = nullptr;
    Vertex m_UI;
};

#endif // HAND_VIEW_LAYER_LAYERMANAGER_H
