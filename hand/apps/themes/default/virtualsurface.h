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
    virtual ~DrawerSdl();

    void Draw(bool forced) override;
    void DrawChild(Layer* child, bool forced) override;

    SDL_Rect CalculateSize(const SDL_Rect& offset) override;
    SDL_Rect GetFrameOffset() override;

    void SetProperties(Collection* config) { m_Properties = config; }

    SDL_Surface* GetBuffer() const { return m_Buffer; }
    virtual void SetBuffer(SDL_Surface* buffer);

    static DrawerSdl* GetDrawer(Layer* from);
    static void BlitSurface(SDL_Surface* source, SDL_Rect* source_pos, SDL_Surface* target);

    /// Removes the visual frame and its allocated spacing, content is extended.
    void RemoveFrame() { m_ShowFrame = false; }

protected:
    void InitBuffer();
    virtual void DrawFrame();
    virtual void DrawBackground();

    // Drawing interface
    void FillRect(SDL_Rect r, const Rgb& color);
    /// Changes x and y of source
    void PlaceCentered(SDL_Surface* source, SDL_Rect& target);

    bool GetDrawFrame() const;
    const Rel_Rect& GetFrameSize() const;
    const Rgb& GetFrameColor() const;
    const Rgb& GetBackgroundColor() const;

    Default* m_Theme;
    Collection* m_Properties;
    bool m_ShowFrame = true;

private:
    SDL_Surface* m_Buffer = nullptr;
};

#endif // HAND_THEME_SDL_BASE_VIRTUALSURFACE_H
