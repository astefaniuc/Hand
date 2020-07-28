#ifndef HAND_VIEW_SCREEN_H
#define HAND_VIEW_SCREEN_H

#include <SDL/SDL.h>
#include "graph/collection.h"
#include "graph/interface.h"


class ModuleLib;

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
    void AddView(HmiItem* viewItem);
    void RemoveView(HmiItem* viewItem);

    bool ShowSurface();

private:
    void InitSDL();

    ModuleLib* m_ThemeLoader;
    // This is the entry point for the whole visualization tree.
    Interface m_View;
    Collection m_Users;
    Collection m_Menu;
    // The whole screen
    SDL_Surface* m_Surface;
    bool m_IsFullscreen;
};

#endif // HAND_VIEW_SCREEN_H
