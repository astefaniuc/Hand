#include "drawersdl.h"
#include "view/layers/list.h"
#include "defines.h"
#include "basicsdl1.h"
#include <SDL2/SDL_ttf.h>


DrawerSdl::DrawerSdl(BasicSdl1* theme, Layer* layer, Hmi::List* config)
    : m_Theme(theme), m_Properties(config)
{
    SetLayer(layer);
    layer->SetDrawer(this);
    m_Theme->Register(this);
}


DrawerSdl::~DrawerSdl()
{
    if (m_Layer)
        m_Layer->RemoveDrawer();
}


void DrawerSdl::Exit(Layer*)
{
    m_Theme->Remove(this);
    m_Layer = nullptr;
    delete this;
}


void DrawerSdl::Draw(SDL_Surface* buffer)
{
    m_Buffer = buffer;
    DrawBackground();
    DrawFrame();

    m_Layer->DrawContent(buffer);
}


DrawerSdl* DrawerSdl::GetDrawer(Layer* from)
{
    return static_cast<DrawerSdl*>(from->GetDrawer());
}


SDL_Rect DrawerSdl::GetContentSize(const SDL_Rect& outer)
{
    SDL_Rect offset = GetFrameOffset();
    SDL_Rect content = outer;
    content.w -= offset.w;
    content.h -= offset.h;
    content.x += offset.x;
    content.y += offset.y;
    return content;
}


SDL_Rect DrawerSdl::GetFramedSize(const SDL_Rect& content)
{
    SDL_Rect offset = GetFrameOffset();
    SDL_Rect total = content;
    total.w += offset.w;
    total.h += offset.h;
    return total;
}


SDL_Rect DrawerSdl::GetFrameOffset()
{
    if (!m_ShowFrame)
        return { 0, 0, 0, 0 };

    SDL_Rect ret;
    const RelRect& frame = GetFrameSize();
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


void DrawerSdl::DrawBackground()
{
    FillRect(m_Layer->GetSize(), GetBackgroundColor());
}


void DrawerSdl::DrawFrame()
{
    if (!GetDrawFrame())
        return;

    // Line width
    int16_t width = m_Theme->GetBaseSize() * 0.1;
    const Rgb& color = GetFrameColor();

    // Draw each line separately
    {
        SDL_Rect top = m_Layer->GetSize();
        top.x -= width;
        top.y -= width;
        top.w += 2 * width;
        top.h = width;
        FillRect(top, color);
    }
    {
        SDL_Rect down = m_Layer->GetSize();
        down.x -= width;
        down.y += down.h;
        down.w += 2 * width;
        down.h = width;
        FillRect(down, color);
    }
    {
        SDL_Rect left = m_Layer->GetSize();
        left.x -= width;
        left.w = width;
        FillRect(left, color);
    }
    {
        SDL_Rect right = m_Layer->GetSize();
        right.x += right.w;
        right.w = width;
        FillRect(right, color);
    }
}


bool DrawerSdl::GetDrawFrame() const
{
    Hmi::TData<bool>* data = dynamic_cast<Hmi::TData<bool>*>(m_Properties->GetChild(DRAWFRAME));
    if (data)
        return data->GetValue();
    return true;
}

const RelRect& DrawerSdl::GetFrameSize() const
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
