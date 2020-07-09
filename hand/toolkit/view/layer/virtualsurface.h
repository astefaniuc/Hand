#ifndef HAND_VIEW_LAYER_VIRTUALSURFACE_H
#define HAND_VIEW_LAYER_VIRTUALSURFACE_H

#include <SDL/SDL.h>
#include "graph/collection.h"
#include "view/datatypes/rect.h"
#include "view/datatypes/rgb.h"


enum buffer_type
{
    NONE,
    OVERLAY,
    COLLECTOR
};


class VirtualSurface
{
public:
    VirtualSurface();
    virtual ~VirtualSurface();

    void Show(SDL_Rect* rect_abs_on_buffer, Rel_Rect* rect_relative_to_parent);

    // Set coordinates and size relative to the parent layer
    virtual void SetSize(SDL_Rect size);
    SDL_Rect GetSize() { return CoordinatesOnBuffer; }

    const Rel_Rect& GetCoordinates() const { return m_Coordinates; }
    void SetCoordinates(const Rel_Rect& value) { m_Coordinates = value; }

    virtual void SetBufferType(buffer_type bt);
    SDL_Surface* GetBuffer();
    virtual void SetBuffer(SDL_Surface* buffer);

    void MapSurface(Rel_Rect* src_rect, SDL_Rect& tgt_rect, SDL_Surface*& tgt_surface);

    void SetParent(VirtualSurface* parent) { m_Parent = parent; }

    void BlitSurface(
            SDL_Surface* source, SDL_Rect* source_pos,
            SDL_Surface* target, SDL_Rect* target_pos);

    bool Changed = true;

protected:
    void FillRect(SDL_Surface* sf, SDL_Rect* r, const Rgb& color);

    buffer_type BufferType = NONE;
    VirtualSurface* m_Parent = nullptr;

    Rel_Rect m_Coordinates;
    SDL_Rect CoordinatesOnBuffer;
    SDL_Surface* Buffer = nullptr;
    bool Updated = false;
};

#endif // HAND_VIEW_LAYER_VIRTUALSURFACE_H
