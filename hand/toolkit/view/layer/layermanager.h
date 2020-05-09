#ifndef HAND_VIEW_LAYER_LAYERMANAGER_H
#define HAND_VIEW_LAYER_LAYERMANAGER_H

#include "view/layer/listlayer.h"


class Device;
class InputState;

/// The MasterLayer is a composed Layer, managing the basic layout and theme.
/// For the sub-layers, the main command list is mandatory; other layers may be
/// additional (system) command lists, a title, a main view, and a secondary view e.g.
/// an info box or help.
class MasterLayer : public Layer
{
public:
    MasterLayer();
    virtual ~MasterLayer();

    bool Update(bool force) override;

    bool GetCommand(Vertex* func, int level);
    void SetDevice(Device* device);

    // Stores the pointer to the object to be displayed next
    bool Show(Vertex* request);

    // VirtualSurface overloads
    void SetSize(SDL_Rect size) { CoordinatesOnBuffer = size; }
    void SetBufferType(buffer_type bt) { BufferType = bt; }
    void SetBuffer(SDL_Surface* buffer) { Buffer = buffer; }


protected:
    void Init() override;

private:
    bool Expand(Vertex* list);
    bool Exit(Vertex* ignore);
    bool GetAllThemes(Vertex* themes_dir);
    bool LoadTheme(Vertex* theme);
    void UnloadTheme();

    // Contains the only pointer to the Device
    Device* _Device = nullptr;
    Vertex* NextRequest = nullptr;
    Layer* MainView = nullptr;
};

#endif // HAND_VIEW_LAYER_LAYERMANAGER_H
