#include "default.h"


extern "C" HmiItem* Create()
{
    return (new Default());
}


extern "C" void Destroy(Theme* theme)
{
    delete theme;
}


typedef Method<Default> Drawer;
typedef LayoutFactory<Default> LayoutF;


Default::Default() : m_Hmi(a_name)
{
    // Drawers
    HmiItem* folder = get(DRAWER);
    folder->set(new Drawer(BACKGROUND, this, &Default::ColorSurface));
    folder->set(new Drawer(FRAME, this, &Default::DrawFrame));
    folder->set(new Drawer(LIST, this, &Default::DrawList));
    folder->set(new Drawer(TEXT, this, &Default::DrawText));

    // Layouts
    folder = new LayoutF(LAYOUT, this, nullptr);
    set(folder);
    folder->set(new LayoutF(BACKGROUND, this, &Default::GetBackgroundLayout));
    folder->set(new LayoutF(BUTTON, this, &Default::GetButtonLayout));
    folder->set(new LayoutF(FRAME, this, &Default::GetFrameLayout));
    folder->set(new LayoutF(LIST, this, &Default::GetListLayout));
    folder->get(LIST)->set(new LayoutF(FRAMEDLIST, this, &Default::GetFramedListLayout));
    folder->get(LIST)->set(new LayoutF(VIEW, this, &Default::GetViewLayout));
    folder->set(new LayoutF(TEXT, this, &Default::GetTextLayout));

    // Properties
    // Dimensions
    folder = get(RECT);
    folder->set(new RectFactory(FRAME,  .01, .03, .98, .94));
    folder->set(new RectFactory(FULL,     0,   0,   1,   1));
    folder->set(new RectFactory(SCALED, .05, .05,  .9,  .9));

    folder = get(ALIGNMENT);
    folder->set(new RectFactory(HORIZONTAL,         1,   0,   1,   0));
    folder->set(new RectFactory(VERTICAL,           0,   1,   0,   1));
    folder->set(new AlternateFactory(ALTERNATE, folder->get(VERTICAL), folder->get(HORIZONTAL)));
    folder->set(new RectFactory(SCALEDHORIZONTAL,   1,   0,  .5,   0));
    // Colors
    folder = get(COLOR);
    folder->get(BACKGROUND)->set(new Rgb(BUTTON,  40,  40, 100));
    folder->get(BACKGROUND)->set(new Rgb(LIST,    20,  20,  50));
    folder->set(new Rgb(FONT,   200, 200, 200));
    folder->set(new Rgb(FRAME,   30,  30,  75));
}


// ------------------------ Layouts -------------------------------


bool Default::GetViewLayout(Layout* layout)
{
    layout->get(COORDINATES)->HmiItem::get(REQUEST)->get(RECT)->get(FULL);
    layout->get(ALIGNMENT)->HmiItem::get(REQUEST)->get(ALIGNMENT)->get(VERTICAL);
    layout->get(DRAWER)->HmiItem::get(REQUEST)->get(DRAWER)->get(LIST);

    layout->get(FIELDS)->get(ELEMENT)->HmiItem::get(REQUEST)->get(LAYOUT)->get(ANY);

    HmiItem* controls = get(LAYOUT)->get(LIST)->get(FRAMEDLIST)->get();
    controls->get(ALIGNMENT)->HmiItem::get(REQUEST)->get(ALIGNMENT)->get()->name(HORIZONTAL);
    controls->get(FIELDS)->get(ELEMENT)->HmiItem::get(REQUEST)->get(LAYOUT)->get()->name(BUTTON);
    layout->add(controls);

    return true;
}


bool Default::GetListLayout(HmiItem* layout)
{
    layout->get(COORDINATES)->HmiItem::get(REQUEST)->get(RECT)->get(FULL);
    layout->get(ALIGNMENT)->HmiItem::get(REQUEST)->get(ALIGNMENT)->get(ALTERNATE);
    layout->get(DRAWER)->HmiItem::get(REQUEST)->get(DRAWER)->get(LIST);

    layout->get(FIELDS)->get(ELEMENT)->HmiItem::get(REQUEST)->get(LAYOUT)->get(ANY);
    return true;
}


bool Default::GetFramedListLayout(HmiItem* layout)
{
    layout->get(COORDINATES)->HmiItem::get(REQUEST)->get(RECT)->get(SCALED);

    HmiItem* frame = get(LAYOUT)->get(FRAME)->get();
    layout->add(frame);

    HmiItem* bgrd = get(LAYOUT)->get(BACKGROUND)->get();
    bgrd->get(COLOR)->HmiItem::get(REQUEST)->get(COLOR)->get(BACKGROUND)->get()->name(LIST);
    frame->add(bgrd);

    frame->add(get(LAYOUT)->get(LIST)->get());
    return true;
}


bool Default::GetButtonLayout(HmiItem* layout)
{
    layout->get(COORDINATES)->HmiItem::get(REQUEST)->get(RECT)->get(SCALED);

    HmiItem* frame = get(LAYOUT)->get(FRAME)->get();
    frame->add(get(LAYOUT)->get(BACKGROUND)->get());
    layout->add(frame);

    // The Button container
    HmiItem* content = get(LAYOUT)->get(LIST)->get();
    content->name("Content");
    content->get(ALIGNMENT)->HmiItem::get(REQUEST)->get(ALIGNMENT)->get()->name(VERTICAL);
    frame->add(content);

    HmiItem* upper = get(LAYOUT)->get(LIST)->get();
    upper->name("Upper");
    upper->get(ALIGNMENT)->HmiItem::get(REQUEST)->get(ALIGNMENT)->get()->name(SCALEDHORIZONTAL);
    content->get(FIELDS)->add(upper);

    HmiItem* lower = get(LAYOUT)->get(LIST)->get();
    lower->name("Lower");
    lower->get(ALIGNMENT)->HmiItem::get(REQUEST)->get(ALIGNMENT)->get()->name(SCALEDHORIZONTAL);
    content->get(FIELDS)->add(lower);

    // Store the layer/layout types for the fields as simple nodes
    upper->get(FIELDS)->get(ICON)->HmiItem::get(REQUEST)->get(LAYOUT)->get(TEXT);
    upper->get(FIELDS)->get(NAME)->HmiItem::get(REQUEST)->get(LAYOUT)->get(TEXT);
    lower->get(FIELDS)->get(DESCRIPTION)->HmiItem::get(REQUEST)->get(LAYOUT)->get(TEXT);
    lower->get(FIELDS)->get(CONTROLID)->HmiItem::get(REQUEST)->get(LAYOUT)->get(LIST);

    return true;
}


bool Default::GetFrameLayout(Layout* layout)
{
    layout->get(COORDINATES)->HmiItem::get(REQUEST)->get(RECT)->get(FRAME);
    layout->get(DRAWER)->HmiItem::get(REQUEST)->get(DRAWER)->get(FRAME);
    layout->get(COLOR)->HmiItem::get(REQUEST)->get(COLOR)->get(FRAME);

    return true;
}


bool Default::GetBackgroundLayout(HmiItem* layout)
{
    layout->get(COORDINATES)->HmiItem::get(REQUEST)->get(RECT)->get(FULL);
    layout->get(DRAWER)->HmiItem::get(REQUEST)->get(DRAWER)->get(BACKGROUND);
    layout->get(COLOR)->HmiItem::get(REQUEST)->get(COLOR)->get(BACKGROUND)->get(BUTTON);

    return true;
}


bool Default::GetTextLayout(HmiItem* layout)
{
    layout->get(COORDINATES)->HmiItem::get(REQUEST)->get(RECT)->get(SCALED);
    layout->get(DRAWER)->HmiItem::get(REQUEST)->get(DRAWER)->get(TEXT);
    layout->get(COLOR)->HmiItem::get(REQUEST)->get(COLOR)->get(FONT);

    return true;
}


// ------------------------- Drawers ------------------------------


bool Default::DrawView(HmiItem* layout)
{
    VirtualSurface* vs = GetSurface(layout);
    if(!vs)
        return false;
    return true;
}


bool Default::DrawText(HmiItem* layout)
{
    VirtualSurface* vs = GetSurface(layout);
    if(!vs)
        return false;

    std::string text = GetString(layout);
    if (text.empty())
    {
        vs->SetBuffer(nullptr);
        return false;
    }

    SDL_Rect size = vs->GetSize();
    // Calculate the fitting font size
    int w, h;
    int fh = size.h * 0.7;
    TTF_SizeText(GetFont(fh), text.c_str(), &w, &h);
    if((w > size.w) || (h > size.h))
        fh = h * double(size.w) * 0.7 / w;

    SDL_Surface* source = RenderText(text, fh, GetRgb("FontColor", layout));

    Rel_Rect sub;
    PlaceCentered(source, size, sub);
    Multiply(&sub, GetRect(COORDINATES, layout));

    vs->SetBuffer(source);
    return true;
}


bool Default::DrawList(HmiItem* layout)
{
    HmiItem* fields = layout->get(FIELDS);
    unsigned cnt = fields->size();
    if(cnt < 1)
        return true;

    Rel_Rect* align = GetRect(ALIGNMENT, layout);
    Rel_Rect* size = GetRect(COORDINATES, layout);
    Rel_Rect calc;

    for(unsigned i=1; i<=cnt; i++)
    {
        double c = double(cnt-i)/double(cnt-i+1);
        calc.w = (1 - (c * align->w)) * (1 - calc.x);
        calc.h = (1 - (c * align->h)) * (1 - calc.y);

        Rel_Rect* sub = GetRect(COORDINATES, fields->get(i));

        // The Rect multiplication is NOT commutative, the order is important
        Rel_Rect tmp = calc;
        Multiply(sub, &tmp);
        *sub = tmp;
        Multiply(size, sub);

        // Set the coordinates for the next iteration
        calc.x += (calc.w * align->x);
        calc.y += (calc.h * align->y);
    }

    return true;
}


bool Default::ColorSurface(HmiItem* layout)
{
    VirtualSurface* vs = GetSurface(layout);
    if(!vs)
        return false;

    SDL_Surface* sf = vs->GetBuffer();
    SDL_Rect size = vs->GetSize();

    Rel_Rect* csap = GetRect(COORDINATES, layout);
    Multiply(csap, &size);
    SDL_SetClipRect(sf, &size);
    FillRect(sf, &size, GetRgb("BackgroundColor", layout));

    return true;
}


std::string Default::GetString(HmiItem* layout)
{
    HmiItem* content = layout->get(TARGET)->get();
    if(content)
        return content->getAsString();
    return "";
}
