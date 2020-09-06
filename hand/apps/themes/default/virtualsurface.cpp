#include "virtualsurface.h"
#include "view/layer/layer.h"
#include "defines.h"
#include <SDL/SDL_ttf.h>


VirtualSurface::~VirtualSurface()
{
    // TODO: Don't delete screen
    SDL_FreeSurface(m_Buffer);
}


void VirtualSurface::Draw(bool a_forced)
{
    InitBuffer();
    DrawBackground();
    DrawFrame();
    DrawSurface();
    DrawChildren(a_forced);
}


VirtualSurface* VirtualSurface::GetDrawer(Layer* a_from)
{
    return dynamic_cast<VirtualSurface*>(a_from->GetDrawer());
}


void VirtualSurface::InitBuffer()
{
    const SDL_Rect& size = m_Layer->GetSize();
    if (m_Buffer && (m_Buffer->w == size.w) && (m_Buffer->h == size.h))
        return; // OK

    SDL_FreeSurface(m_Buffer);

    const SDL_PixelFormat& fmt = *(SDL_GetVideoSurface()->format);
    m_Buffer = SDL_CreateRGBSurface(
            SDL_DOUBLEBUF|SDL_HWSURFACE, size.w, size.h,
            fmt.BitsPerPixel, fmt.Rmask, fmt.Gmask, fmt.Bmask, 0xFF000000);
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
    FillRect(GetContentSize(), GetBackgroundColor());
}


void VirtualSurface::DrawFrame()
{
    if (!GetDrawFrame())
        return;

    SDL_Rect total = m_Layer->GetSize();
    SDL_Rect content = GetContentSize();
    // Line width
    int16_t width = (total.w > total.h ? total.w - content.w : total.h - content.h) * 0.1;
    const Rgb& color = GetFrameColor();
    // Draw each line separately
    {
        SDL_Rect top = content;
        top.x -= width;
        top.y -= width;
        top.w += 2 * width;
        top.h = width;
        FillRect(top, color);
    }
    {
        SDL_Rect down = content;
        down.x -= width;
        down.y += content.h;
        down.w += 2 * width;
        down.h = width;
        FillRect(down, color);
    }
    {
        SDL_Rect left = content;
        left.x -= width;
        left.w = width;
        FillRect(left, color);
    }
    {
        SDL_Rect right = content;
        right.x += content.w;
        right.w = width;
        FillRect(right, color);
    }
}


bool VirtualSurface::GetDrawFrame() const
{
    TData<bool>* data = dynamic_cast<TData<bool>*>(m_Properties->GetChild(DRAWFRAME));
    if (data)
        return data->GetValue();
    return true;
}

const Rel_Rect& VirtualSurface::GetFrameSize() const
{
    return ((Rect*)(m_Properties->GetChild(FRAMESIZE)))->GetValue();
}

const Rgb& VirtualSurface::GetFrameColor() const
{
    return *((Rgb*)m_Properties->GetChild(FRAMECOLOR));
}

const Rgb& VirtualSurface::GetBackgroundColor() const
{
    return *((Rgb*)m_Properties->GetChild(BACKGROUNDCOLOR));
}


SDL_Rect VirtualSurface::GetContentSize()
{
    if (!m_ShowFrame)
        return m_Layer->GetSize();
    return Multiply(GetFrameSize(), m_Layer->GetSize());
}


void VirtualSurface::BlitSurface(
    SDL_Surface* source, SDL_Rect* src_pos, SDL_Surface* target, SDL_Rect* tgt_pos)
{
    if (source && target)
    {
        SDL_SetClipRect(target, src_pos);
        SDL_BlitSurface(source, nullptr, target, src_pos);
    }
}


void VirtualSurface::FillRect(SDL_Rect r, const Rgb& c)
{
    SDL_SetClipRect(m_Buffer, &r);
    SDL_FillRect(m_Buffer, &r, SDL_MapRGB(m_Buffer->format, c.m_r, c.m_g, c.m_b));
}


void VirtualSurface::PlaceCentered(SDL_Surface* a_src, SDL_Rect& a_tgt)
{
    a_tgt.x += (a_tgt.w - a_src->w) / 2;
    a_tgt.y += (a_tgt.h - a_src->h) / 2;
    a_tgt.w = a_src->w;
    a_tgt.h = a_src->h;
}


void VirtualSurface::SetBuffer(SDL_Surface* buffer)
{
    SDL_FreeSurface(m_Buffer);
    m_Buffer = buffer;
}
