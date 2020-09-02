#include "virtualsurface.h"
#include "view/layer/layer.h"
#include "defines.h"
#include "default.h"
#include <SDL/SDL_ttf.h>


VirtualSurface::~VirtualSurface()
{
    // TODO: Don't delete screen
    SDL_FreeSurface(m_Buffer);
}


void VirtualSurface::Draw(bool a_forced)
{
    SDL_Rect surfaceSize = m_Layer->GetSize();
    if (m_Buffer && (m_Buffer->w != surfaceSize.w) && (m_Buffer->h != surfaceSize.h))
    {
        SDL_FreeSurface(m_Buffer);
        m_Buffer = nullptr;
    }

    DrawBackground();
    DrawSurface();
    DrawChildren(a_forced);
}


VirtualSurface* VirtualSurface::GetDrawer(Layer* a_from)
{
    return dynamic_cast<VirtualSurface*>(a_from->GetDrawer());
}


bool VirtualSurface::DrawChildren(bool a_forced)
{
    bool ret = false;
    for (Layer* sublayer : m_Layer->GetSubLayers())
    {
        ret |= sublayer->Draw(a_forced);

        SDL_Rect srcRect = sublayer->GetSize();
        SDL_Rect tgtPos = m_Layer->GetSize();

        VirtualSurface* src = GetDrawer(sublayer);
        if (src)
            BlitSurface(src->GetBuffer(), &srcRect, GetBuffer(), &tgtPos);
        // TODO: else
    }
    return ret;
}


void VirtualSurface::DrawBackground()
{
    SDL_Rect size = GetContentSize(m_Layer->GetSize());

    SDL_SetClipRect(GetBuffer(), &size);
    FillRect(GetBuffer(), &size, GetBackgroundColor());
}


void VirtualSurface::DrawFrame()
{
    SDL_Rect total_size = m_Layer->GetSize();
    SDL_Rect content_size = GetContentSize(total_size);

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


SDL_Rect VirtualSurface::GetContentSize(const SDL_Rect& totalSpace)
{
    SDL_Rect tmp = totalSpace;
    // Remove the parent positioning, the returned rect is relative to the current rect
    tmp.x = tmp.y = 0;
    return Multiply(GetFrameSize(), tmp);
}


void VirtualSurface::BlitSurface(
    SDL_Surface* source, SDL_Rect* src_pos, SDL_Surface* target, SDL_Rect* tgt_pos)
{
    // Recalculate also the position
    SDL_SetClipRect(target, src_pos);
    if (source && target)
    {
//        SDL_SetAlpha(source, SDL_SRCALPHA, 128);
//        SDL_SetAlpha(target, SDL_SRCALPHA, 128);
        SDL_BlitSurface(source, nullptr, target, src_pos);
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


void VirtualSurface::PlaceCentered(SDL_Surface* a_src, SDL_Rect& a_tgt)
{
    a_tgt.x += (a_tgt.w - a_src->w) / 2;
    a_tgt.y += (a_tgt.h - a_src->h) / 2;
    a_tgt.w = a_src->w;
    a_tgt.h = a_src->h;
}


SDL_Surface* VirtualSurface::GetBuffer()
{
    if (!m_Buffer)
    {
        // TODO ...
        m_Buffer = SDL_GetVideoSurface();
        const SDL_PixelFormat& fmt = *(m_Buffer->format);
        SDL_Rect surfSize = m_Layer->GetSize();
        // Draw on buffer independent of the position
        m_Buffer = SDL_CreateRGBSurface(
                SDL_DOUBLEBUF|SDL_HWSURFACE,
                surfSize.w, surfSize.h,
                fmt.BitsPerPixel,
                fmt.Rmask, fmt.Gmask, fmt.Bmask, fmt.Amask);
    }
    return m_Buffer;
}


void VirtualSurface::SetBuffer(SDL_Surface* buffer)
{
    SDL_FreeSurface(m_Buffer);
    m_Buffer = buffer;
}
