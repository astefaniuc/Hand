#ifndef HAND_VIEW_SCREEN_H
#define HAND_VIEW_SCREEN_H

#include <SDL/SDL.h>
#include "base/module.h"
#include "layer/user.h"


class Screen
{
public:
    Screen();

    // App mode interface
    bool ToggleFullscreen(Vertex* ignore);
    bool SetFullscreen();
    bool SetWindowed();
    void Init();
    // GetSize()/ GetPosition()
    SDL_Rect GetResolution();
    // GetBuffer()
    // Re-calculate position for all layer manager
    bool SetLayerManagerPositions();
    SDL_Surface* GetSurface() { return Surface; }

    bool ShowSurface();

    void SetLayerManager(LayerManager* a_lm) { m_LayerManager = a_lm; }
    LayerManager* GetLayerManager() { return m_LayerManager; }

private:
    void InitSDL();

    // The whole screen
    SDL_Surface* Surface;
    // TODO: we need only one "Size" member and the "IsFullscreen" to store
    // all sizes we need ("Surface" stores the second size)
    bool IsFullscreen;

    LayerManager* m_LayerManager;
};

#endif // HAND_VIEW_SCREEN_H
