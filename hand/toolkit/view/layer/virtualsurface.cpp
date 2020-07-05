#include "view/layer/virtualsurface.h"


VirtualSurface::VirtualSurface()
{
    CoordinatesOnBuffer.x = CoordinatesOnBuffer.y =
            CoordinatesOnBuffer.w = CoordinatesOnBuffer.h = 0;
}


VirtualSurface::~VirtualSurface()
{
    SDL_FreeSurface(Buffer);
}


void VirtualSurface::MapSurface(Rel_Rect* src_rect, SDL_Rect& tgt_rect, SDL_Surface*& tgt_surface)
{
    if(BufferType == COLLECTOR)
    {
        // Get the absolute position on the current buffer
        tgt_rect = CoordinatesOnBuffer;
        Multiply(*src_rect, tgt_rect);
        tgt_surface = GetBuffer();
        // "Updated" must be true in this case
        return;
    }

    // TODO: surface mapped incorrectly for Collection having BufferType OVERLAY

    // Get the next layers buffer and absolute position
    Rel_Rect sap = GetCoordinates();
    Multiply(*src_rect, sap);
    SetCoordinates(sap);
    if(m_Parent)
        m_Parent->MapSurface(src_rect, tgt_rect, tgt_surface);
}


void VirtualSurface::Show(SDL_Rect* a_absOnBuffer, Rel_Rect* a_relativeToParent)
{
    if(Updated || !m_Parent)
        return;

    SDL_Rect tgt_rect;
    // Calculate position of the excerpt on the parent layer
    SDL_Surface* tgt_surface = nullptr;
    m_Parent->MapSurface(a_relativeToParent, tgt_rect, tgt_surface);

    if(m_Parent->BufferType == OVERLAY)
        m_Parent->Show(&tgt_rect, a_relativeToParent);

    // DrawObject as parameter for positioning and alpha values
    BlitSurface(Buffer, a_absOnBuffer, tgt_surface, &tgt_rect);
}


void VirtualSurface::BlitSurface(
    SDL_Surface* source, SDL_Rect* src_pos, SDL_Surface* target, SDL_Rect* tgt_pos)
{
    // Recalculate also the position
    SDL_SetClipRect(target, tgt_pos);
    if(source && target)
    {
//                SDL_SetAlpha(source, SDL_SRCALPHA, 128);
//                SDL_SetAlpha(target, SDL_SRCALPHA, 128);
        SDL_BlitSurface(source, src_pos, target, tgt_pos);
    }
}


void VirtualSurface::SetSize(SDL_Rect size)
{
    // Store only the size, position from layout
    Multiply(GetCoordinates(), size);
    if ((size.w != CoordinatesOnBuffer.w) || (size.h != CoordinatesOnBuffer.h))
    {
        CoordinatesOnBuffer.w = size.w;
        CoordinatesOnBuffer.h = size.h;
        SDL_FreeSurface(Buffer);
        Buffer = nullptr;
        Changed = true;
    }
}


void VirtualSurface::FillRect(SDL_Surface* sf, SDL_Rect* r, const Rgb& c)
{
    SDL_FillRect(sf, r, SDL_MapRGB(sf->format, c.m_r, c.m_g, c.m_b));
}


void VirtualSurface::SetBufferType(buffer_type bt)
{
    if((BufferType != NONE) && (bt == NONE))
        SDL_FreeSurface(Buffer);

    BufferType = bt;
}


SDL_Surface* VirtualSurface::GetBuffer()
{
    if (!Buffer)
    {
        // TODO ...
        Buffer = SDL_GetVideoSurface();
        const SDL_PixelFormat& fmt = *(Buffer->format);
        // Draw on buffer independent of the position
        Buffer = SDL_CreateRGBSurface(
                SDL_DOUBLEBUF|SDL_HWSURFACE,
                CoordinatesOnBuffer.w, CoordinatesOnBuffer.h,
                fmt.BitsPerPixel,
                fmt.Rmask, fmt.Gmask, fmt.Bmask, fmt.Amask);
    }
    return Buffer;
}


void VirtualSurface::SetBuffer(SDL_Surface* buffer)
{
    SDL_FreeSurface(Buffer);
    Buffer = buffer;
}
