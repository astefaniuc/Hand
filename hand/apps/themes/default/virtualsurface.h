#ifndef HAND_THEME_SDL_BASE_VIRTUALSURFACE_H
#define HAND_THEME_SDL_BASE_VIRTUALSURFACE_H

#include <SDL/SDL.h>
#include "graph/collection.h"
#include "view/drawer.h"
#include "view/datatypes/rect.h"
#include "view/datatypes/rgb.h"


class Default;

class DrawerSdl : public Drawer
{
public:
    DrawerSdl(Default* theme, Collection* config) : m_Theme(theme), m_Properties(config) {}

    void Draw(SDL_Surface* buffer) override;
    void DrawChild(Layer* child) override;

    SDL_Rect CalculateSize(SDL_Rect& content) override;
    SDL_Rect GetFrameOffset() override;

    void SetProperties(Collection* config) { m_Properties = config; }

    static DrawerSdl* GetDrawer(Layer* from);
    static void BlitSurface(SDL_Surface* source, SDL_Rect* source_pos, SDL_Surface* target);
    static void PlaceCentered(const SDL_Rect& source, SDL_Rect& target);

    /// Removes the visual frame and its allocated spacing, content is extended.
    void RemoveFrame() { m_ShowFrame = false; }

protected:
    virtual void DrawFrame();
    virtual void DrawBackground();

    SDL_Rect GetFramedSize(SDL_Rect& content);
    // Drawing interface
    void FillRect(SDL_Rect r, const Rgb& color);
    /// Changes x and y of source

    bool GetDrawFrame() const;
    const Rel_Rect& GetFrameSize() const;
    const Rgb& GetFrameColor() const;
    const Rgb& GetBackgroundColor() const;

    Default* m_Theme;
    SDL_Surface* m_Buffer = nullptr;
    Collection* m_Properties;
    bool m_ShowFrame = true;
};

#endif // HAND_THEME_SDL_BASE_VIRTUALSURFACE_H
