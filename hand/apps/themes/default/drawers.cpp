#include "drawers.h"
#include "defines.h"
#include "view/layer/layer.h"
#include "view/layout.h"
// #include "SDL/SDL_Image.h"


void ButtonDrawer::DrawSurface()
{
    // TODO
}


void ListDrawer::DrawSurface()
{
}


void TextDrawer::DrawSurface()
{
    if (m_ViewItem.empty())
    {
        SetBuffer(nullptr);
        return;
    }

    SDL_Rect size = m_Layer->GetSize();
    // Calculate the fitting font size
    int w, h;
    int fh = size.h * 0.7;
    TTF_SizeText(m_Theme->GetFont(fh), m_ViewItem.c_str(), &w, &h);
    if((w > size.w) || (h > size.h))
        fh = h * double(size.w) * 0.7 / w;

    SDL_Surface* txt = RenderText(m_ViewItem, fh, GetFontColor());

    Rel_Rect sub;
    PlaceCentered(txt, size, sub);
//    Multiply(&sub, GetFrameSize());

    SetBuffer(txt);
}

const Rgb& TextDrawer::GetFontColor()
{
    return *((Rgb*)m_Properties->GetChild(FONTCOLOR));
}


void ViewDrawer::DrawSurface()
{
    // TODO
}

