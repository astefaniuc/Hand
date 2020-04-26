#include "default.h"


extern "C" Vertex* Create()
{
    return (new Default());
}


extern "C" void Destroy(Theme* theme)
{
    delete theme;
}


typedef Method<Default> Drawer;
typedef LayoutFactory<Default> LayoutF;


Default::Default() : Theme(DEFAULT)
{
    // Drawers
    Vertex* folder = get(DRAWER);
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
    folder                 ->set(new Rgb(FONT,   200, 200, 200));
    folder                 ->set(new Rgb(FRAME,   30,  30,  75));
}


// ------------------------ Layouts -------------------------------


bool Default::GetViewLayout(Vertex* layout)
{
    layout->get(COORDINATES)->Vertex::get(REQUEST)->get(RECT)->get(FULL);
    layout->get(ALIGNMENT)->Vertex::get(REQUEST)->get(ALIGNMENT)->get(VERTICAL);
    layout->get(DRAWER)->Vertex::get(REQUEST)->get(DRAWER)->get(LIST);

    layout->get(FIELDS)->get(ELEMENT)->Vertex::get(REQUEST)->get(LAYOUT)->get(ANY);

    Vertex* controls = get(LAYOUT)->get(LIST)->get(FRAMEDLIST)->get();
    controls->get(ALIGNMENT)->Vertex::get(REQUEST)->get(ALIGNMENT)->get()->name(HORIZONTAL);
    controls->get(FIELDS)->get(ELEMENT)->Vertex::get(REQUEST)->get(LAYOUT)->get()->name(BUTTON);
    layout->add(controls);

    return true;
}


bool Default::GetListLayout(Vertex* layout)
{
    layout->get(COORDINATES)->Vertex::get(REQUEST)->get(RECT)->get(FULL);
    layout->get(ALIGNMENT)->Vertex::get(REQUEST)->get(ALIGNMENT)->get(ALTERNATE);
    layout->get(DRAWER)->Vertex::get(REQUEST)->get(DRAWER)->get(LIST);

    layout->get(FIELDS)->get(ELEMENT)->Vertex::get(REQUEST)->get(LAYOUT)->get(ANY);
    return true;
}


bool Default::GetFramedListLayout(Vertex* layout)
{
    layout->get(COORDINATES)->Vertex::get(REQUEST)->get(RECT)->get(SCALED);

    Vertex* frame = get(LAYOUT)->get(FRAME)->get();
    layout->add(frame);

    Vertex* bgrd = get(LAYOUT)->get(BACKGROUND)->get();
    bgrd->get(COLOR)->Vertex::get(REQUEST)->get(COLOR)->get(BACKGROUND)->get()->name(LIST);
    frame->add(bgrd);

    frame->add(get(LAYOUT)->get(LIST)->get());
    return true;
}


bool Default::GetButtonLayout(Vertex* layout)
{
    layout->get(COORDINATES)->Vertex::get(REQUEST)->get(RECT)->get(SCALED);

    Vertex* frame = get(LAYOUT)->get(FRAME)->get();
    frame->add(get(LAYOUT)->get(BACKGROUND)->get());
    layout->add(frame);

    // The Button container
    Vertex* content = get(LAYOUT)->get(LIST)->get();
    content->name("Content");
    content->get(ALIGNMENT)->Vertex::get(REQUEST)->get(ALIGNMENT)->get()->name(VERTICAL);
    frame->add(content);

    Vertex* upper = get(LAYOUT)->get(LIST)->get();
    upper->name("Upper");
    upper->get(ALIGNMENT)->Vertex::get(REQUEST)->get(ALIGNMENT)->get()->name(SCALEDHORIZONTAL);
    content->get(FIELDS)->add(upper);

    Vertex* lower = get(LAYOUT)->get(LIST)->get();
    lower->name("Lower");
    lower->get(ALIGNMENT)->Vertex::get(REQUEST)->get(ALIGNMENT)->get()->name(SCALEDHORIZONTAL);
    content->get(FIELDS)->add(lower);

    // Store the layer/layout types for the fields as simple nodes
    upper->get(FIELDS)->get(ICON)->Vertex::get(REQUEST)->get(LAYOUT)->get(TEXT);
    upper->get(FIELDS)->get(NAME)->Vertex::get(REQUEST)->get(LAYOUT)->get(TEXT);
    lower->get(FIELDS)->get(DESCRIPTION)->Vertex::get(REQUEST)->get(LAYOUT)->get(TEXT);
    lower->get(FIELDS)->get(CONTROLID)->Vertex::get(REQUEST)->get(LAYOUT)->get(LIST);

    return true;
}


bool Default::GetFrameLayout(Vertex* layout)
{
    layout->get(COORDINATES)->Vertex::get(REQUEST)->get(RECT)->get(FRAME);
    layout->get(DRAWER)->Vertex::get(REQUEST)->get(DRAWER)->get(FRAME);
    layout->get(COLOR)->Vertex::get(REQUEST)->get(COLOR)->get(FRAME);

    return true;
}


bool Default::GetBackgroundLayout(Vertex* layout)
{
    layout->get(COORDINATES)->Vertex::get(REQUEST)->get(RECT)->get(FULL);
    layout->get(DRAWER)->Vertex::get(REQUEST)->get(DRAWER)->get(BACKGROUND);
    layout->get(COLOR)->Vertex::get(REQUEST)->get(COLOR)->get(BACKGROUND)->get(BUTTON);

    return true;
}


bool Default::GetTextLayout(Vertex* layout)
{
    layout->get(COORDINATES)->Vertex::get(REQUEST)->get(RECT)->get(SCALED);
    layout->get(DRAWER)->Vertex::get(REQUEST)->get(DRAWER)->get(TEXT);
    layout->get(COLOR)->Vertex::get(REQUEST)->get(COLOR)->get(FONT);

    return true;
}


// ------------------------- Drawers ------------------------------


bool Default::DrawFrame(Vertex* layout)
{
    VirtualSurface* vs = GetSurface(layout);
    if(!vs)
        return false;

    SDL_Rect total_size = vs->GetSize();
    SDL_Rect content_size = total_size;
    Rel_Rect* sap = GetRect(COORDINATES, layout);
    Multiply(sap, &content_size);

    Vertex* fields = layout->get(FIELDS);
    Vertex* child;
    uint i = 0;
    while((child=fields->get(++i)) != nullptr)
        Multiply(sap, GetRect(COORDINATES, child));

    // Draw each frame line separately
    SDL_Rect up, down, left, right;
    up.x = total_size.x;
    up.y = total_size.y;
    up.w = total_size.w;
    up.h = content_size.y - total_size.y;

    down.x = total_size.x;
    down.y = content_size.y + content_size.h;
    down.w = total_size.w;
    down.h = total_size.h - content_size.h - up.h;

    left.x = total_size.x;
    left.y = content_size.y;
    left.w = content_size.x - total_size.x;
    left.h = content_size.h;

    right.x = content_size.x + content_size.w;
    right.y = left.y;
    right.w = total_size.w - left.w - content_size.w;
    right.h = left.h;

    SDL_Surface* buffer = vs->GetBuffer();
    SDL_Rect* border[4] = {&up, &down, &left, &right};
    Rgb* color = GetRgb("FrameColor", layout);
    for(unsigned i = 0; i < 4; ++i)
    {
        SDL_SetClipRect(buffer, border[i]);
        FillRect(buffer, border[i], color);
    }
    return true;
}


bool Default::DrawView(Vertex* layout)
{
    VirtualSurface* vs = GetSurface(layout);
    if(!vs)
        return false;
    return true;
}


bool Default::DrawText(Vertex* layout)
{
    VirtualSurface* vs = GetSurface(layout);
    if(!vs)
        return false;

    std::string text = GetString(layout);
    if(text.empty())
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


bool Default::DrawList(Vertex* layout)
{
    Vertex* fields = layout->get(FIELDS);
    uint cnt = fields->size();
    if(cnt < 1)
        return true;

    Rel_Rect* align = GetRect(ALIGNMENT, layout);
    Rel_Rect* size = GetRect(COORDINATES, layout);
    Rel_Rect calc;

    for(uint i=1; i<=cnt; i++)
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


bool Default::ColorSurface(Vertex* layout)
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


std::string Default::GetString(Vertex* layout)
{
    Vertex* content = layout->get(TARGET)->get();
    if(content)
        return content->getAsString();
    return "";
}


//================================================//
/*
ZoomIn::ZoomIn()
{
    Progress = 0.1;
    AnimationRate = 0.1;
}


bool ZoomIn::GetSize(SDL_Rect& size)
{
    if(!GetNextProgress()) return false;
    Rect tmp;
    tmp.w = tmp.h = tmp.x = tmp.y = Progress;
    SqueezeRect(size, tmp);
    return true;

}

bool ZoomIn::GetNextProgress()
{
    if(Progress == 1.0)
        return false;
    if(Progress < 1.0)
        //progress = 50;
        Progress += AnimationRate;
    else if(Progress > 1.0)
        Progress -= AnimationRate;
    return true;
}
*/
