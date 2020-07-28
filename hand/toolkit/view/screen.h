#ifndef HAND_VIEW_SCREEN_H
#define HAND_VIEW_SCREEN_H

#include <SDL/SDL.h>
#include "layer/user.h"
#include "graph/collection.h"


class Screen
{
public:
    Screen();

    // App mode interface
    void ToggleFullscreen(HmiItem*);
    bool SetFullscreen();
    bool SetWindowed();
    SDL_Rect GetResolution();

    // Re-calculate position for all layer manager
    void Add(CUser* client);
    void Remove(CUser* client);

    bool ShowSurface();

private:
    void InitSDL();

    Collection m_Users;
    // The whole screen
    SDL_Surface* m_Surface;
    bool m_IsFullscreen;
};

#endif // HAND_VIEW_SCREEN_H
