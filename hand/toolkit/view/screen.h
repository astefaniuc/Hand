#ifndef HAND_VIEW_SCREEN_H
#define HAND_VIEW_SCREEN_H

#include <SDL/SDL.h>
#include <vector>
#include "base/module.h"
#include "layer/user.h"


class Screen
{
public:
    Screen();

    // App mode interface
    void ToggleFullscreen(HmiItem*);
    bool SetFullscreen();
    bool SetWindowed();
    // GetSize()/ GetPosition()
    SDL_Rect GetResolution();
    // GetBuffer()

    // Re-calculate position for all layer manager
    void Add(CUser* client);
    void Remove(CUser* client);

    bool ShowSurface();

private:
    void InitSDL();
    void SplitScreen();

    std::vector<CUser*> m_Users;
    // The whole screen
    SDL_Surface* m_Surface;
    // TODO: we need only one "Size" member and the "IsFullscreen" to store
    // all sizes we need ("Surface" stores the second size)
    bool m_IsFullscreen;
};

#endif // HAND_VIEW_SCREEN_H
