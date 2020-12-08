#include "text.h"
#include "defines.h"
#include "basicsdl1.h"
#include "view/layers/text.h"
// #include "SDL/SDL_Image.h"


SDL_Rect Text::CalculateSize(SDL_Rect& content)
{
    const std::string& text = static_cast<Layers::Text*>(m_Layer)->GetData();

    if (text.empty())
        content.w = content.h = 0;
    else
    {
        // Type mismatch in the SDL lib
        int w, h;
        TTF_SizeText(m_Theme->GetFont(GetFontSize()), text.c_str(), &w, &h);
        content.w = (uint16_t)w;
        content.h = (uint16_t)h;
    }

    return GetFramedSize(content);
}


void Text::Draw(SDL_Surface* buffer)
{
    const std::string& text = static_cast<Layers::Text*>(m_Layer)->GetData();
    if (text.empty())
        return;

    if (m_Layer->IsModified())
    {
        SDL_FreeSurface(m_Buffer);
        m_Buffer = RenderText(text, GetFontSize(), GetFontColor());
    }

    SDL_Rect srcRect = m_Layer->GetContentSize();
    BlitSurface(m_Buffer, &srcRect, buffer);
}


SDL_Surface* Text::RenderText(const std::string& text, int size, const Rgb& color) const
{
    SDL_Color sdl_color;
    sdl_color.r = color.m_r;
    sdl_color.g = color.m_g;
    sdl_color.b = color.m_b;
    return TTF_RenderText_Blended(m_Theme->GetFont(size), text.c_str(), sdl_color);
}


const Rgb& Text::GetFontColor() const
{
    return *((Rgb*)m_Properties->GetChild(FONTCOLOR));
}


unsigned Text::GetFontSize() const
{
    return ((Hmi::TData<unsigned>*)(m_Properties->GetChild(FONTSIZE)))->GetValue();
}
