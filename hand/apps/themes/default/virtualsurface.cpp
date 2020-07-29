#include "virtualsurface.h"
#include "view/layer/layer.h"
#include "defines.h"
#include "default.h"
#include <SDL/SDL_ttf.h>


VirtualSurface::~VirtualSurface()
{
    // TODO: Don't delete screen
    SDL_FreeSurface(Buffer);
}


void VirtualSurface::Draw(bool forced)
{
/*  if(BufferType == NONE)
    {
        // TODO: this crashes in MasterLayer during "write through"
        Parent->MapSurface(_Layout->Coordinates,
                            CoordinatesOnBuffer,
                            GetBuffer());
        Updated = true; // ?
    }*/
    // Call the Theme function for drawing with the current settings

    // derived drawing
    DrawSurface();

    if (GetBufferType() == COLLECTOR)
        // Draw first the child on the buffer
        m_Layer->ShowChildren(forced);

    Rel_Rect tmp = m_Layer->GetLayout()->GetCoordinates();
    SDL_Rect tgtRect = m_Layer->GetSize();
    Show(&tgtRect, &tmp);

    if (GetBufferType() == OVERLAY)
    {
        // Draw childs afterwards (not buffered)
        m_Layer->ShowChildren(forced);
//        Updated = true;
    }
}


void VirtualSurface::DrawBackground()
{
    SDL_Surface* sf = GetBuffer();
    SDL_Rect size = m_Layer->GetSize();

    const Rel_Rect& csap = GetFrameSize();
    Multiply(csap, size);
    SDL_SetClipRect(sf, &size);
    FillRect(sf, &size, GetBackgroundColor());
}


void VirtualSurface::DrawFrame()
{
    SDL_Rect total_size = m_Layer->GetSize();
    SDL_Rect content_size = total_size;
    Multiply(GetFrameSize(), content_size);

    // Draw each frame line separately
    SDL_Rect up, down, left, right;
    up.x = total_size.x;
    up.y = total_size.y;
    up.w = total_size.w;
    up.h = content_size.y - total_size.y;

    down.x = total_size.x;
    down.y = content_size.y + content_size.h;
    down.w = total_size.w;
    down.h = total_size.h - content_size.h - up.h;

    left.x = total_size.x;
    left.y = content_size.y;
    left.w = content_size.x - total_size.x;
    left.h = content_size.h;

    right.x = content_size.x + content_size.w;
    right.y = left.y;
    right.w = total_size.w - left.w - content_size.w;
    right.h = left.h;

    SDL_Surface* buffer = GetBuffer();
    SDL_Rect* border[4] = { &up, &down, &left, &right };
    for (unsigned i = 0; i < 4; ++i)
    {
        SDL_SetClipRect(buffer, border[i]);
        FillRect(buffer, border[i], GetFrameColor());
    }
}

const Rel_Rect& VirtualSurface::GetFrameSize()
{
    return *((Rel_Rect*)m_Properties->GetChild(FRAMESIZE));
}

const Rgb& VirtualSurface::GetFrameColor()
{
    return *((Rgb*)m_Properties->GetChild(FRAMECOLOR));
}

const Rgb& VirtualSurface::GetBackgroundColor()
{
    return *((Rgb*)m_Properties->GetChild(BACKGROUNDCOLOR));
}


void VirtualSurface::MapSurface(Rel_Rect* src_rect, SDL_Rect& tgt_rect, SDL_Surface*& tgt_surface)
{
    if (BufferType == COLLECTOR)
    {
        // Get the absolute position on the current buffer
        tgt_rect = m_Layer->GetSize();
        Multiply(*src_rect, tgt_rect);
        tgt_surface = GetBuffer();
        // "Updated" must be true in this case
        return;
    }

    // TODO: surface mapped incorrectly for Collection having BufferType OVERLAY

    // Get the next layers buffer and absolute position
    Rel_Rect sap = m_Layer->GetLayout()->GetCoordinates();
    Multiply(*src_rect, sap);
    if (m_Parent)
        m_Parent->MapSurface(src_rect, tgt_rect, tgt_surface);
    else
        tgt_surface = SDL_GetVideoSurface();
}


void VirtualSurface::Show(SDL_Rect* a_absOnBuffer, Rel_Rect* a_relativeToParent)
{
    if (Updated || !m_Parent)
        return;

    SDL_Rect tgt_rect;
    // Calculate position of the excerpt on the parent layer
    SDL_Surface* tgt_surface = nullptr;
    m_Parent->MapSurface(a_relativeToParent, tgt_rect, tgt_surface);

    if (m_Parent->BufferType == OVERLAY)
        m_Parent->Show(&tgt_rect, a_relativeToParent);

    // DrawObject as parameter for positioning and alpha values
    BlitSurface(Buffer, a_absOnBuffer, tgt_surface, &tgt_rect);
}


void VirtualSurface::BlitSurface(
    SDL_Surface* source, SDL_Rect* src_pos, SDL_Surface* target, SDL_Rect* tgt_pos)
{
    // Recalculate also the position
    SDL_SetClipRect(target, tgt_pos);
    if (source && target)
    {
//                SDL_SetAlpha(source, SDL_SRCALPHA, 128);
//                SDL_SetAlpha(target, SDL_SRCALPHA, 128);
        SDL_BlitSurface(source, src_pos, target, tgt_pos);
    }
}


void VirtualSurface::FillRect(SDL_Surface* sf, SDL_Rect* r, const Rgb& c)
{
    SDL_FillRect(sf, r, SDL_MapRGB(sf->format, c.m_r, c.m_g, c.m_b));
}


SDL_Surface* VirtualSurface::RenderText(const std::string& text, int size, const Rgb& color)
{
    SDL_Color sdl_color;
    sdl_color.r = color.m_r;
    sdl_color.g = color.m_g;
    sdl_color.b = color.m_b;
    return TTF_RenderText_Blended(m_Theme->GetFont(size), text.c_str(), sdl_color);
}


void VirtualSurface::PlaceCentered(SDL_Surface* source, SDL_Rect& target, Rel_Rect& out)
{
    out.w = double(source->w) / double(target.w);
    out.h = double(source->h) / double(target.h);
    out.x = (1 - out.w) / 2;
    out.y = (1 - out.h) / 2;
}


void VirtualSurface::SetBufferType(buffer_type bt)
{
    if ((BufferType != NONE) && (bt == NONE))
        SDL_FreeSurface(Buffer);

    BufferType = bt;
}


SDL_Surface* VirtualSurface::GetBuffer()
{
    SDL_Rect surfaceSize = m_Layer->GetSize();
    if (Buffer && (Buffer->w != surfaceSize.w) && (Buffer->h != surfaceSize.h))
    {
        SDL_FreeSurface(Buffer);
        Buffer = nullptr;
    }

    if (!Buffer)
    {
        // TODO ...
        Buffer = SDL_GetVideoSurface();
        const SDL_PixelFormat& fmt = *(Buffer->format);
        SDL_Rect surfSize = m_Layer->GetSize();
        // Draw on buffer independent of the position
        Buffer = SDL_CreateRGBSurface(
                SDL_DOUBLEBUF|SDL_HWSURFACE,
                surfSize.w, surfSize.h,
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
