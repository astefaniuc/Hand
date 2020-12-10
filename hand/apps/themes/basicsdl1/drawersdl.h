#ifndef HAND_THEME_BASICSDL1_DRAWERSDL_H
#define HAND_THEME_BASICSDL1_DRAWERSDL_H

#include <SDL/SDL.h>
#include "data/list.h"
#include "view/drawer.h"
#include "view/datatypes/rect.h"
#include "view/datatypes/rgb.h"


class BasicSdl1;

class DrawerSdl : public Drawer
{
public:
    DrawerSdl(BasicSdl1* theme, Hmi::List* config) : m_Theme(theme), m_Properties(config) {}

    void Draw(SDL_Surface* buffer) override;

    SDL_Rect ComputeSize(const SDL_Rect& content) override { return GetFramedSize(content); }
    SDL_Rect GetContentSize(const SDL_Rect& outer) override;

    void SetProperties(Hmi::List* config) { m_Properties = config; }

    static DrawerSdl* GetDrawer(Layer* from);
    static void BlitSurface(SDL_Surface* source, SDL_Rect* source_pos, SDL_Surface* target);

    /// Removes the visual frame and its allocated spacing, content is extended.
    void RemoveFrame() { m_ShowFrame = false; }

protected:
    virtual void DrawFrame();
    virtual void DrawBackground();

    SDL_Rect GetFrameOffset();
    SDL_Rect GetFramedSize(const SDL_Rect& content);
    // Drawing interface
    void FillRect(SDL_Rect r, const Rgb& color);
    /// Changes x and y of source

    bool GetDrawFrame() const;
    const RelRect& GetFrameSize() const;
    const Rgb& GetFrameColor() const;
    const Rgb& GetBackgroundColor() const;

    BasicSdl1* m_Theme;
    SDL_Surface* m_Buffer = nullptr;
    Hmi::List* m_Properties;
    bool m_ShowFrame = true;
};

#endif // HAND_THEME_BASICSDL1_DRAWERSDL_H
