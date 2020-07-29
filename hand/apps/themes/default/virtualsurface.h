#ifndef HAND_VIEW_VIRTUALSURFACE_H
#define HAND_VIEW_VIRTUALSURFACE_H

#include <SDL/SDL.h>
//#include "graph/collection.h"
#include "view/drawer.h"
#include "default.h"
#include "view/datatypes/rect.h"
#include "view/datatypes/rgb.h"
#include "graph/collection.h"


enum buffer_type
{
    NONE,
    OVERLAY,
    COLLECTOR
};


class VirtualSurface : public Drawer
{
public:
    VirtualSurface(Default* theme)
        : m_Theme(theme) { SetBufferType(COLLECTOR); }
    virtual ~VirtualSurface();

    void SetProperties(Collection* config) { m_Properties = config; }
    Theme* GetParentTheme() override { return m_Theme; }

    void Draw(bool forced) override;
    void Show(SDL_Rect* rect_abs_on_buffer, Rel_Rect* rect_relative_to_parent);

    void SetBufferType(buffer_type bt);
    buffer_type GetBufferType() { return BufferType; }

    SDL_Surface* GetBuffer();
    virtual void SetBuffer(SDL_Surface* buffer);

    void SetParent(VirtualSurface* parent) { m_Parent = parent; }

    void MapSurface(Rel_Rect* src_rect, SDL_Rect& tgt_rect, SDL_Surface*& tgt_surface);
    void BlitSurface(
            SDL_Surface* source, SDL_Rect* source_pos,
            SDL_Surface* target, SDL_Rect* target_pos);

protected:
    virtual void DrawSurface() = 0;

    void FillRect(SDL_Surface* sf, SDL_Rect* r, const Rgb& color);

    virtual void DrawFrame();
    virtual void DrawBackground();
    // Drawing interface
    virtual SDL_Surface* RenderText(const std::string& text, int size, const Rgb& color);
    // Changes x and y of source
    void PlaceCentered(SDL_Surface* source, SDL_Rect& target, Rel_Rect& out);

    const Rel_Rect& GetFrameSize();

    const Rgb& GetFrameColor();
    const Rgb& GetBackgroundColor();

    Default* m_Theme;
    Collection* m_Properties = nullptr;
    buffer_type BufferType = NONE;
    VirtualSurface* m_Parent = nullptr;

    SDL_Surface* Buffer = nullptr;
    bool Updated = false;
};

#endif // HAND_VIEW_VIRTUALSURFACE_H
