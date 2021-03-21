#ifndef HAND_THEMES_BASICSDL1_H
#define HAND_THEMES_BASICSDL1_H

#include "view/theme.h"
#include "data/map.h"
#include "data/vector.h"
#include "text.h"
#include <SDL2/SDL_ttf.h>
#include <map>


#define FONT_FILE "/usr/share/fonts/truetype/freefont/FreeSans.ttf"


class Drawer;

class BasicSdl1 : public Theme
{
public:
    BasicSdl1();
    ~BasicSdl1();

    void GetHmi(Layer* caller) override;

    void InitScreen(Layer* root) override;
    void UpdateScreen(bool forced) override;

    virtual TTF_Font* GetFont(int size);
    /// Frames are scaled based on this.
    unsigned GetBaseSize();

    Drawer* GetButtonDrawer(Layer* layer) override { return new DrawerSdl(this, layer, m_Buttons); }
    Drawer* GetDataDrawer(Layer* layer) override { return new DrawerSdl(this, layer, m_Datas); }
    Drawer* GetChordDrawer(Layer* layer) override { return new DrawerSdl(this, layer, m_Chords); }
    Drawer* GetListDrawer(Layer* layer) override { return new DrawerSdl(this, layer, m_Lists); }
    Drawer* GetTextDrawer(Layer* layer) override { return new Text(this, layer, m_Texts); }
    Drawer* GetViewDrawer(Layer* layer) override { return new DrawerSdl(this, layer, m_Views); }

    void Register(DrawerSdl* child) { m_Drawers.push_back(child); }
    void Remove(DrawerSdl* child);

protected:
    // App mode interface
    void ToggleFullscreen(Layers::Item*);
    SDL_Rect GetResolution();

    Hmi::Vector m_Hmi;
    Hmi::Map* m_Buttons;
    Hmi::Map* m_Datas;
    Hmi::Map* m_Chords;
    Hmi::Map* m_Lists;
    Hmi::Map* m_Texts;
    Hmi::Map* m_Views;

    Layer* m_ScreenRoot = nullptr;
    // The whole screen TODO: can we use SDL_GetVideoSurface() instead?
    SDL_Window* m_Window = nullptr;

    // Stores once rendered fonts
    std::map<int, TTF_Font*> Fonts;
    std::vector<DrawerSdl*> m_Drawers;
};

#endif // HAND_THEMES_BASICSDL1_H
