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


void VirtualSurface::Draw(bool a_forced)
{
    DrawBackground();
    DrawSurface();
    DrawChildren(a_forced);
}


bool VirtualSurface::DrawChildren(bool a_forced)
{
    bool ret = false;
    for (Layer* sublayer : m_Layer->GetSubLayers())
    {
        ret |= sublayer->Draw(a_forced);

        SDL_Rect srcRect = sublayer->GetSize();
        SDL_Rect tgtPos = m_Layer->GetSize();

        VirtualSurface* src = dynamic_cast<VirtualSurface*>(sublayer->GetDrawer());
        if (src)
            BlitSurface(src->GetBuffer(), &srcRect, GetBuffer(), &tgtPos);
        // TODO: else
    }
    return ret;
}


void VirtualSurface::DrawBackground()
{
    SDL_Rect size = m_Layer->GetSize();
    Multiply(GetFrameSize(), size);

    SDL_SetClipRect(GetBuffer(), &size);
    FillRect(GetBuffer(), &size, GetBackgroundColor());
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
    return ((Rect*)(m_Properties->GetChild(FRAMESIZE)))->GetValue();
}

const Rgb& VirtualSurface::GetFrameColor()
{
    return *((Rgb*)m_Properties->GetChild(FRAMECOLOR));
}

const Rgb& VirtualSurface::GetBackgroundColor()
{
    return *((Rgb*)m_Properties->GetChild(BACKGROUNDCOLOR));
}


void VirtualSurface::BlitSurface(
    SDL_Surface* source, SDL_Rect* src_pos, SDL_Surface* target, SDL_Rect* tgt_pos)
{
    // Recalculate also the position
    SDL_SetClipRect(target, tgt_pos);
    if (source && target)
    {
//        SDL_SetAlpha(source, SDL_SRCALPHA, 128);
//        SDL_SetAlpha(target, SDL_SRCALPHA, 128);
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
