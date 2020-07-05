#include <iostream>
#include "view/theme/theme.h"
#include "view/datatypes/rgb.h"
#include "view/datatypes/rect.h"
#include "view/layer/virtualsurface.h"


Theme::Theme()
{
    if(TTF_Init() == -1)
    {
        std::cout << TTF_GetError() << std::endl;
        exit(22);
    }
}


Theme::~Theme()
{
    std::map<int, TTF_Font*>::iterator curr = Fonts.begin();
    while(curr!=Fonts.end())
    {
        TTF_CloseFont((*curr).second);
        curr++;
    }
}


bool Theme::Draw(Layout* a_input)
{
    if(!a_input)
        return false;

    // TODO
    return true;
}


TTF_Font* Theme::GetFont(int size)
{
    // Loading font:
    TTF_Font* font = Fonts[size];
    if(!font)
    {
        font = TTF_OpenFont(FONT_FILE, size);
        if(!font)
        {
            std::cout << "Unable to load font:" << FONT_FILE
                    << "\nTTF_OpenFont: " << TTF_GetError() << std::endl;
            exit(25);
        }
        Fonts[size] = font;
    }
    return font;
}

/*
void Theme::GetFontHeight(HmiItem* layout, unsigned& max_size)
{
    HmiItem* pref = layout->get(DIM_FONT_PREFERRED);
    if (pref)
        max_size = ((Data<unsigned>*)pref)->get();
    else
    {
        unsigned max = ((Data<unsigned>*)layout->get(DIM_FONT_MAX))->get();
        if(max_size >= max)
            max_size = max;
    }
}*/


SDL_Surface* Theme::RenderText(const std::string& text, int size, Rgb* color)
{
    SDL_Color sdl_color;
    sdl_color.r = color->m_r;
    sdl_color.g = color->m_g;
    sdl_color.b = color->m_b;
    return TTF_RenderText_Blended(GetFont(size), text.c_str(), sdl_color);
}


void Theme::PlaceCentered(SDL_Surface* source, SDL_Rect& target, Rel_Rect& out)
{
    out.w = double(source->w) / double(target.w);
    out.h = double(source->h) / double(target.h);
    out.x = (1 - out.w) / 2;
    out.y = (1 - out.h) / 2;
}
