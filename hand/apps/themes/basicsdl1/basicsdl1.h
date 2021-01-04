#ifndef HAND_THEMES_BASICSDL1_H
#define HAND_THEMES_BASICSDL1_H

#include "view/theme.h"
#include "data/map.h"
#include "data/vector.h"
#include "text.h"
#include <SDL/SDL_ttf.h>
#include <map>


#define FONT_FILE "/usr/share/fonts/truetype/freefont/FreeSans.ttf"


class Drawer;

class BasicSdl1 : public Theme
{
public:
    BasicSdl1();
    ~BasicSdl1();

    Hmi::Item* GetHmi() override { return &m_Hmi; }

    void InitScreen(Layer* root) override;
    void UpdateScreen() override;

    virtual TTF_Font* GetFont(int size);
    /// Frames are scaled based on this.
    unsigned GetBaseSize();

    Drawer* GetButtonDrawer() override { return new DrawerSdl(this, m_Buttons); }
    Drawer* GetDataDrawer() override { return new DrawerSdl(this, m_Datas); }
    Drawer* GetListDrawer() override { return new DrawerSdl(this, m_Lists); }
    Drawer* GetTextDrawer() override { return new Text(this, m_Texts); }
    Drawer* GetViewDrawer() override { return new DrawerSdl(this, m_Views); }

protected:
    // App mode interface
    void ToggleFullscreen(Hmi::Item*);
    void SetFullscreen();
    void SetWindowed();
    SDL_Rect GetResolution();

    Hmi::Vector m_Hmi;
    Hmi::Map* m_Buttons;
    Hmi::Map* m_Datas;
    Hmi::Map* m_Lists;
    Hmi::Map* m_Texts;
    Hmi::Map* m_Views;

    Layer* m_ScreenRoot = nullptr;
    // The whole screen TODO: can we use SDL_GetVideoSurface() instead?
    SDL_Surface* m_Surface = nullptr;
    bool m_IsFullscreen = false;
    // Stores once rendered fonts
    std::map<int, TTF_Font*> Fonts;
};

#endif // HAND_THEMES_BASICSDL1_H
