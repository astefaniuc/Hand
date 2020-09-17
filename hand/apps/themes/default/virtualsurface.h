#ifndef HAND_THEME_SDL_BASE_VIRTUALSURFACE_H
#define HAND_THEME_SDL_BASE_VIRTUALSURFACE_H

#include <SDL/SDL.h>
#include "graph/collection.h"
#include "view/drawer.h"
#include "view/datatypes/rect.h"
#include "view/datatypes/rgb.h"


class VirtualSurface : public Drawer
{
public:
    VirtualSurface(Collection* config) : m_Properties(config) {}
    virtual ~VirtualSurface();

    void Draw(bool forced) override;
    SDL_Rect GetContentSize() override;

    void SetProperties(Collection* config) { m_Properties = config; }

    SDL_Surface* GetBuffer() const { return m_Buffer; }
    virtual void SetBuffer(SDL_Surface* buffer);

    static VirtualSurface* GetDrawer(Layer* from);

    /// Removes the visual frame and its allocated spacing, content is extended.
    void RemoveFrame() { m_ShowFrame = false; }

protected:
    void InitBuffer();
    virtual void DrawFrame();
    virtual void DrawBackground();
    void DrawChildren(bool forced);

    // Drawing interface
    void FillRect(SDL_Rect r, const Rgb& color);
    void BlitSurface(SDL_Surface* source, SDL_Rect* source_pos, SDL_Surface* target);
    /// Changes x and y of source
    void PlaceCentered(SDL_Surface* source, SDL_Rect& target);

    bool GetDrawFrame() const;
    const Rel_Rect& GetFrameSize() const;
    const Rgb& GetFrameColor() const;
    const Rgb& GetBackgroundColor() const;

    Collection* m_Properties = nullptr;
    bool m_ShowFrame = true;

private:
    SDL_Surface* m_Buffer = nullptr;
};

#endif // HAND_THEME_SDL_BASE_VIRTUALSURFACE_H
