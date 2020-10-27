#ifndef HAND_THEMES_BASICSDL1_H
#define HAND_THEMES_BASICSDL1_H

#include "view/theme.h"
#include "data/collection.h"
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

    HmiItem* GetHmi() override { return &m_Hmi; }

    void InitScreen(Layer* root) override;
    void UpdateScreen() override;

    virtual TTF_Font* GetFont(int size);
    /// Frames are scaled based on this.
    unsigned GetBaseSize();

    Drawer* GetButtonDrawer() override { return new DrawerSdl(this, &m_Buttons); }
    Drawer* GetDataDrawer() override { return new DrawerSdl(this, &m_Datas); }
    Drawer* GetListDrawer() override { return new DrawerSdl(this, &m_Lists); }
    Drawer* GetTextDrawer() override { return new Text(this, &m_Texts); }
    Drawer* GetViewDrawer() override { return new DrawerSdl(this, &m_Views); }

protected:
    // App mode interface
    void ToggleFullscreen(HmiItem*);
    void SetFullscreen();
    void SetWindowed();
    SDL_Rect GetResolution();

    Collection m_Hmi;
    Collection m_Buttons;
    Collection m_Datas;
    Collection m_Lists;
    Collection m_Texts;
    Collection m_Views;

    Layer* m_ScreenRoot = nullptr;
    // The whole screen TODO: can we use SDL_GetVideoSurface() instead?
    SDL_Surface* m_Surface = nullptr;
    bool m_IsFullscreen = false;
    // Stores once rendered fonts
    std::map<int, TTF_Font*> Fonts;
};

#endif // HAND_THEMES_BASICSDL1_H
