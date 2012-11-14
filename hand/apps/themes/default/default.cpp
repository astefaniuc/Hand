/*
 *  Copyright 2012 Alex Stefaniuc
 *
 *  This file is part of Hand.
 *
 *  Hand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  Hand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with Hand. If not, see <http://www.gnu.org/licenses/>.
 */

#include "default.h"


using namespace std;


extern "C" Vertex* Create()
{
    return (new Default());
}


extern "C" void Destroy(Theme* theme)
{
    delete theme;
}


typedef Method<Default> Functoid;


Default::Default() : Theme("DefaultTheme")
{
    // Drawers
    Vertex* folder = get("Repository")->get(DRAWER);
    folder->set(new Functoid(BACKGROUND, this, &Default::ColorSurface));
    folder->set(new Functoid(FRAME,      this, &Default::DrawFrame));
    folder->set(new Functoid(LIST,       this, &Default::DrawList));
    folder->set(new Functoid(TEXT,       this, &Default::DrawText));

    // Layouts
    folder = get("Repository")->get(LAYOUT);
    folder->set(new Functoid(BACKGROUND, this, &Default::GetBackgroundLayout));
    folder->set(new Functoid(BUTTON,     this, &Default::GetButtonLayout));
    folder->set(new Functoid(CONTROLID,  this, &Default::GetControlLayout));
    folder->set(new Functoid(FRAME,      this, &Default::GetFrameLayout));
    folder->set(new Functoid(FRAMEDLIST, this, &Default::GetFramedListLayout));
    folder->set(new Functoid(LIST,       this, &Default::GetListLayout));
    folder->set(new Functoid(TEXT,       this, &Default::GetTextLayout));
    folder->set(new Functoid(VIEW,       this, &Default::GetViewLayout));

    // Properties
    // Dimensions
    folder = get("Repository")->get(RECT);
    folder->set(new RectFactory(FRAME,            .01, .03, .98, .94));
    folder->set(new RectFactory(FULL,               0,   0,   1,   1));
    folder->set(new RectFactory(HORIZONTAL,         1,   0,   1,   0));
    folder->set(new RectFactory(SCALED,           .05, .05,  .9,  .9));
    folder->set(new RectFactory(SCALEDHORIZONTAL,   1,   0,  .5,   0));
    folder->set(new RectFactory(VERTICAL,           0,   1,   0,   1));
    // Colors
    folder = get("Repository")->get(COLOR);
    folder->get(BACKGROUND)->set(new Rgb(BUTTON,  40,  40, 100));
    folder->get(BACKGROUND)->set(new Rgb(LIST,    20,  20,  50));
    folder                 ->set(new Rgb(FONT,   200, 200, 200));
    folder                 ->set(new Rgb(FRAME,   30,  30,  75));
}

// ----------------------------------------------------------------
// ------------------------ Layouts -------------------------------
// ----------------------------------------------------------------


bool Default::GetViewLayout(Vertex* layout)
{
    layout->get(SIZEANDPOSITION)->Vertex::get(REQUEST)->get(RECT)->get(FULL);
    layout->get(ALIGNMENT)->Vertex::get(REQUEST)->get(RECT)->get(VERTICAL);
    layout->get(DRAWER)->Vertex::get(REQUEST)->get(DRAWER)->get(LIST);

    layout->get(FIELDS)->get(ELEMENT)->get(ANY);

    Vertex* controls = layout->get(LAYOUT, FRAMEDLIST);
    controls->get(ALIGNMENT)->Vertex::get(REQUEST)->get(RECT)->get(HORIZONTAL);

    controls->get(FIELDS)->get(ELEMENT)->get(BUTTON);
    return true;
}


bool Default::GetListLayout(Vertex* layout)
{
    layout->get(SIZEANDPOSITION)->Vertex::get(REQUEST)->get(RECT)->get(FULL);
    layout->get(ALIGNMENT)->Vertex::get(REQUEST)->get(RECT)->get(VERTICAL);
    layout->get(DRAWER)->Vertex::get(REQUEST)->get(DRAWER)->get(LIST);

    layout->get(FIELDS)->get(ELEMENT)->get(ANY);

    return true;
}


bool Default::GetFramedListLayout(Vertex* layout)
{
    layout->get(SIZEANDPOSITION)->Vertex::get(REQUEST)->get(RECT)->get(SCALED);

    Vertex* bgrd = layout->get(LAYOUT, BACKGROUND);
    bgrd->get(COLOR)->Vertex::get(REQUEST)->get(COLOR)->get(BACKGROUND)->get(LIST);

    // The content
    Vertex* content = layout->get(LAYOUT, LIST);
    bgrd->get(TOUPDATE)->attach(content);

    return true;
}


bool Default::GetButtonLayout(Vertex* layout)
{
    layout->get(SIZEANDPOSITION)->Vertex::get(REQUEST)->get(RECT)->get(SCALED);

    Vertex* frame = layout->get(LAYOUT, FRAME);

    Vertex* bgrd = frame->get(LAYOUT, BACKGROUND);
    frame->get(TOUPDATE)->attach(bgrd);

    // The Button container
    Vertex* content = layout->get(LAYOUT, LIST);
    content->get(ALIGNMENT)->Vertex::get(REQUEST)->get(RECT)->get(VERTICAL);
    frame->get(TOUPDATE)->attach(content);

    Vertex* upper = content->get(LAYOUT, LIST);
    // The next call of content->get(LAYOUT, LIST) should return a new object
    upper->name("Upper");
    upper->get(ALIGNMENT)->Vertex::get(REQUEST)->get(RECT)->get(SCALEDHORIZONTAL);

    Vertex* lower =content->get(LAYOUT, LIST);
    lower->name("Lower");
    lower->get(ALIGNMENT)->Vertex::get(REQUEST)->get(RECT)->get(SCALEDHORIZONTAL);

    // The fields for the next level of layers
    upper->get(FIELDS)->set(new Layout(ICON, TEXT));
    upper->get(FIELDS)->set(new Layout(NAME, TEXT));
    lower->get(FIELDS)->set(new Layout(DESCRIPTION, TEXT));
    lower->get(FIELDS)->set(new Layout(CONTROLID, LIST));

    return true;
}


bool Default::GetControlLayout(Vertex* layout)
{
    layout->get(SIZEANDPOSITION)->Vertex::get(REQUEST)->get(RECT)->get(SCALED);

    Vertex* frame = layout->get(LAYOUT, FRAME);

    Vertex* bgrd = frame->get(LAYOUT, BACKGROUND);
    frame->get(TOUPDATE)->attach(bgrd);

    // The Button container
    Vertex* content = layout->get(LAYOUT, TEXT);
    frame->get(TOUPDATE)->attach(content);

    return true;
}


bool Default::GetFrameLayout(Vertex* layout)
{
    layout->get(SIZEANDPOSITION)->Vertex::get(REQUEST)->get(RECT)->get(FRAME);
    layout->get(DRAWER)->Vertex::get(REQUEST)->get(DRAWER)->get(FRAME);
    layout->get(COLOR)->Vertex::get(REQUEST)->get(COLOR)->get(FRAME);

    return true;
}


bool Default::GetBackgroundLayout(Vertex* layout)
{
    layout->get(SIZEANDPOSITION)->Vertex::get(REQUEST)->get(RECT)->get(FULL);
    layout->get(DRAWER)->Vertex::get(REQUEST)->get(DRAWER)->get(BACKGROUND);
    layout->get(COLOR)->Vertex::get(REQUEST)->get(COLOR)->get(BACKGROUND)->get(BUTTON);

    return true;
}


bool Default::GetTextLayout(Vertex* layout)
{
    layout->get(SIZEANDPOSITION)->Vertex::get(REQUEST)->get(RECT)->get(SCALED);
    layout->get(DRAWER)->Vertex::get(REQUEST)->get(DRAWER)->get(TEXT);
    layout->get(COLOR)->Vertex::get(REQUEST)->get(COLOR)->get(FONT);

    return true;
}


// ----------------------------------------------------------------
// ------------------------- Drawers ------------------------------
// ----------------------------------------------------------------


bool Default::DrawFrame(Vertex* layout)
{
    VirtualSurface* vs = GetSurface(layout);
    if(!vs)
        return false;

    SDL_Rect total_size = vs->GetSize();
    SDL_Rect content_size = total_size;
    Rel_Rect* sap = GetRect(SIZEANDPOSITION, layout);
    Multiply(sap, &content_size);

    Vertex* to_update = layout->get(TOUPDATE);
    if(to_update)
    {
        Vertex* child;
        uint i = 0;
        while((child=to_update->get(++i)) != NULL)
            Multiply(sap, GetRect(SIZEANDPOSITION, child));
    }

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
    for(int i=0; i<4; i++)
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

    string text = GetString(layout);
    if(text.empty())
    {
        vs->SetBuffer(NULL);
        return false;
    }

    SDL_Rect size = vs->GetSize();
    // Calculate the fitting font size
    int w, h;
    int fh = size.h*0.7;
    TTF_SizeText(GetFont(fh), text.c_str(), &w, &h);
    if((w>size.w) || (h>size.h))
        fh = h*double(size.w)*0.7/w;

    SDL_Surface* source = RenderText(&text, fh, GetRgb("FontColor", layout));

    Rel_Rect sub;
    PlaceCentered(source, size, sub);
    Multiply(&sub, GetRect(SIZEANDPOSITION, layout));

    vs->SetBuffer(source);
    return true;
}


bool Default::DrawList(Vertex* layout)
{
    Vertex* children = layout->get(TOUPDATE);
    if(!children)
        return false;

    uint child_cnt = children->size();
    if(child_cnt < 1)
        return true;

    Rel_Rect* align = GetRect(ALIGNMENT, layout);
    Rel_Rect* size = GetRect(SIZEANDPOSITION, layout);
    Rel_Rect calc;

    for(uint i=1; i<=child_cnt; i++)
    {
        double c = double(child_cnt-i)/double(child_cnt-i+1);
        calc.w = (1 - (c * align->w))*(1 - calc.x);
        calc.h = (1 - (c * align->h))*(1 - calc.y);

        Rel_Rect* sub = GetRect(SIZEANDPOSITION, children->get(i));

        // The Rect multiplication is NOT commutative, the order is important
        Rel_Rect tmp = calc;
        Multiply(sub, &tmp);
        *sub = tmp;
        Multiply(size, sub);

        // Set the coordinates for the next iteration
        calc.x += (calc.w*align->x);
        calc.y += (calc.h*align->y);
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

    Rel_Rect* csap = GetRect(SIZEANDPOSITION, layout);
    Multiply(csap, &size);
    SDL_SetClipRect(sf, &size);
    FillRect(sf, &size, GetRgb("BackgroundColor", layout));

    return true;
}


string Default::GetString(Vertex* layout)
{
    Vertex* content = layout->get(TARGET)->get();
    if(content)
        return content->getAsString();
    return "";
}


//================================================//
//================================================//
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
