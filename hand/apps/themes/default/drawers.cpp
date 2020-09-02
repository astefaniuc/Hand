#include "drawers.h"
#include "defines.h"
#include "view/layer/layer.h"
#include "view/layer/textlayer.h"
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
    const std::string& text = dynamic_cast<TextLayer*>(m_Layer)->GetData();
    if (text.empty())
    {
        SetBuffer(nullptr);
        return;
    }

    SDL_Rect size = m_Layer->GetSize();
    // Calculate the fitting font size
    int w, h;
    int fh = size.h * 0.7;
    TTF_SizeText(m_Theme->GetFont(fh), text.c_str(), &w, &h);
    if ((w > size.w) || (h > size.h))
        fh = h * double(size.w) * 0.7 / w;

    SetBuffer(RenderText(text, fh, GetFontColor()));
    PlaceCentered(GetBuffer(), size);
    m_Layer->SetSize(size);
}


const Rgb& TextDrawer::GetFontColor()
{
    return *((Rgb*)m_Properties->GetChild(FONTCOLOR));
}


void ViewDrawer::DrawSurface()
{
    // TODO
}

