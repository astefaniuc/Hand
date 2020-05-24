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
    HmiItem* ToggleFullscreen(HmiItem*);
    bool SetFullscreen();
    bool SetWindowed();
    void Init();
    // GetSize()/ GetPosition()
    SDL_Rect GetResolution();
    // GetBuffer()
    // Re-calculate position for all layer manager
    bool SetLayerManagerPositions();
    // TODO: handle multiple users
    SDL_Surface* GetSurface(CUser*) { return Surface; }

    bool ShowSurface();

private:
    void InitSDL();

    // The whole screen
    SDL_Surface* Surface;
    // TODO: we need only one "Size" member and the "IsFullscreen" to store
    // all sizes we need ("Surface" stores the second size)
    bool IsFullscreen;
};

#endif // HAND_VIEW_SCREEN_H
