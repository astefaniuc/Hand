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
    const std::vector<Layer*>& fields = m_Layer->GetSubLayers();
    unsigned cnt = fields.size();
    if (!cnt)
        return;

    ListLayout* layout = dynamic_cast<ListLayout*>(m_Layer->GetLayout());
    const Rel_Rect& align = layout->GetAlignment();
    const Rel_Rect& size = layout->GetCoordinates();
    Rel_Rect calc;

    for (unsigned i = 0; i < cnt; ++i)
    {
        double c = double(cnt - i) / double(cnt - i + 1);
        calc.w = (1 - (c * align.w)) * (1 - calc.x);
        calc.h = (1 - (c * align.h)) * (1 - calc.y);

        const Rel_Rect& sub = fields[i]->GetLayout()->GetCoordinates();

        // The Rect multiplication is NOT commutative, the order is important
        Rel_Rect tmp = calc;
        Multiply(sub, tmp);
//        *sub = tmp;
//        Multiply(size, sub);

        // Set the coordinates for the next iteration
        calc.x += (calc.w * align.x);
        calc.y += (calc.h * align.y);
    }
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

