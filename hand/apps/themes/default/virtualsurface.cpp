#include "virtualsurface.h"
#include "view/layer/layer.h"
#include "defines.h"
#include "default.h"
#include <SDL/SDL_ttf.h>


DrawerSdl::~DrawerSdl()
{
    // TODO: Don't delete screen
    SDL_FreeSurface(m_Buffer);
}


void DrawerSdl::Draw(bool a_forced)
{
    InitBuffer();
    DrawBackground();
    DrawFrame();
    m_Layer->DrawChildren(a_forced);
}


DrawerSdl* DrawerSdl::GetDrawer(Layer* a_from)
{
    return static_cast<DrawerSdl*>(a_from->GetDrawer());
}


void DrawerSdl::InitBuffer()
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


SDL_Rect DrawerSdl::GetFramedSize(SDL_Rect& content, const SDL_Rect& offset)
{
    SDL_Rect total = content;
    total.w += offset.w;
    total.h += offset.h;
    content.x += offset.x;
    content.y += offset.y;
    return total;
}


SDL_Rect DrawerSdl::CalculateSize(SDL_Rect& content, SDL_Rect& total)
{
    content = m_Layer->GetLayoutSize();
    total = GetFramedSize(content, GetFrameOffset());

    return total;
}


SDL_Rect DrawerSdl::GetFrameOffset()
{
    if (!m_ShowFrame)
        return { 0, 0, 0, 0 };

    SDL_Rect ret;
    const Rel_Rect& frame = GetFrameSize();
    unsigned base = m_Theme->GetBaseSize();

    ret.w = base * frame.w;
    ret.h = base * frame.h;
    ret.x = ret.w * frame.x;
    ret.y = ret.h * frame.y;

    return ret;
}

void DrawerSdl::BlitSurface(SDL_Surface* source, SDL_Rect* src_pos, SDL_Surface* target)
{
    if (source && target)
    {
        SDL_SetClipRect(target, src_pos);
        SDL_BlitSurface(source, nullptr, target, src_pos);
    }
}


void DrawerSdl::DrawChild(Layer* child, bool a_forced)
{
    child->Draw(a_forced);

    SDL_Rect srcRect = child->GetSize();
    DrawerSdl* src = GetDrawer(child);
    if (src)
        BlitSurface(src->GetBuffer(), &srcRect, GetBuffer());
    // TODO: else
}


void DrawerSdl::DrawBackground()
{
    FillRect(m_Layer->GetContentSize(), GetBackgroundColor());
}


void DrawerSdl::DrawFrame()
{
    if (!GetDrawFrame())
        return;

    SDL_Rect total = m_Layer->GetSize();
    SDL_Rect content = m_Layer->GetContentSize();
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


bool DrawerSdl::GetDrawFrame() const
{
    TData<bool>* data = dynamic_cast<TData<bool>*>(m_Properties->GetChild(DRAWFRAME));
    if (data)
        return data->GetValue();
    return true;
}

const Rel_Rect& DrawerSdl::GetFrameSize() const
{
    return ((Rect*)(m_Properties->GetChild(FRAMESIZE)))->GetValue();
}

const Rgb& DrawerSdl::GetFrameColor() const
{
    return *((Rgb*)m_Properties->GetChild(FRAMECOLOR));
}

const Rgb& DrawerSdl::GetBackgroundColor() const
{
    return *((Rgb*)m_Properties->GetChild(BACKGROUNDCOLOR));
}


void DrawerSdl::FillRect(SDL_Rect r, const Rgb& c)
{
    SDL_SetClipRect(m_Buffer, &r);
    SDL_FillRect(m_Buffer, &r, SDL_MapRGB(m_Buffer->format, c.m_r, c.m_g, c.m_b));
}


void DrawerSdl::PlaceCentered(SDL_Surface* a_src, SDL_Rect& a_tgt)
{
    a_tgt.x += (a_tgt.w - a_src->w) / 2;
    a_tgt.y += (a_tgt.h - a_src->h) / 2;
    a_tgt.w = a_src->w;
    a_tgt.h = a_src->h;
}


void DrawerSdl::SetBuffer(SDL_Surface* buffer)
{
    SDL_FreeSurface(m_Buffer);
    m_Buffer = buffer;
}
