#include "drawers.h"
#include "defines.h"
#include "default.h"
#include "view/layer/textlayer.h"
// #include "SDL/SDL_Image.h"


SDL_Rect TextDrawer::CalculateSize(const SDL_Rect& offset)
{
    SDL_Rect content = { 0, 0, 0, 0 };
    const std::string& text = static_cast<TextLayer*>(m_Layer)->GetData();

    if (!text.empty())
    {
        // Type mismatch in the SDL lib
        int w, h;
        TTF_SizeText(m_Theme->GetFont(GetFontSize()), text.c_str(), &w, &h);
        content.w = (uint16_t)w;
        content.h = (uint16_t)h;
    }

    return content;
}


void TextDrawer::Draw(bool)
{
    const std::string& text = static_cast<TextLayer*>(m_Layer)->GetData();
    if (text.empty())
    {
        SetBuffer(nullptr);
        return;
    }
    InitBuffer();
    DrawBackground();

    SDL_Surface* render = RenderText(text, GetFontSize(), GetFontColor());
    SDL_Rect srcRect = m_Layer->GetContentSize();
    BlitSurface(render, &srcRect, GetBuffer());
    SDL_FreeSurface(render);
}


SDL_Surface* TextDrawer::RenderText(const std::string& text, int size, const Rgb& color) const
{
    SDL_Color sdl_color;
    sdl_color.r = color.m_r;
    sdl_color.g = color.m_g;
    sdl_color.b = color.m_b;
    return TTF_RenderText_Blended(m_Theme->GetFont(size), text.c_str(), sdl_color);
}


const Rgb& TextDrawer::GetFontColor() const
{
    return *((Rgb*)m_Properties->GetChild(FONTCOLOR));
}


unsigned TextDrawer::GetFontSize() const
{
    return ((TData<unsigned>*)(m_Properties->GetChild(FONTSIZE)))->GetValue();
}
