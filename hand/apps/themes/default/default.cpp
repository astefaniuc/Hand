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


Default::Default() : Theme("DefaultTheme")
{
    // Drawer factories
    Register(new DrawerFactory(new Method<Default>(GUI_DRAWER_LIST, this, &Default::DrawList)));
    Register(new DrawerFactory(new Method<Default>(GUI_DRAWER_BUTTON, this, &Default::DrawButton)));
    Register(new DrawerFactory(new Method<Default>(GUI_DRAWER_FRAME, this, &Default::DrawFrame)));
    Register(new DrawerFactory(new Method<Default>(GUI_DRAWER_BACKGROUND, this, &Default::ColorSurface)));
    Register(new DrawerFactory(new Method<Default>(GUI_DRAWER_TEXT, this, &Default::DrawText)));

    // Layouts
    Register(new LayoutFactory<Default>(VIEW, this, &Default::GetViewLayout));
    Register(new LayoutFactory<Default>(FRAMEDLIST, this, &Default::GetFramedListLayout));
    Register(new LayoutFactory<Default>(LIST, this, &Default::GetListLayout));
    Register(new LayoutFactory<Default>(CONTROLID, this, &Default::GetControlLayout));
    Register(new LayoutFactory<Default>(BUTTON, this, &Default::GetButtonLayout));
    Register(new LayoutFactory<Default>(FRAME, this, &Default::GetFrameLayout));
    Register(new LayoutFactory<Default>(BACKGROUND, this, &Default::GetBackgroundLayout));
    Register(new LayoutFactory<Default>(TEXT, this, &Default::GetTextLayout));

    // Colors
    Register(new PropertyFactory<Default>(GUI_COLOR_FRAME, this, &Default::GetColorFrame));
    Register(new PropertyFactory<Default>(GUI_COLOR_BACKGR_LIST, this, &Default::GetColorBgrdList));
    Register(new PropertyFactory<Default>(GUI_COLOR_BACKGR_BUTTON, this, &Default::GetColorBgrdButton));
    Register(new PropertyFactory<Default>(GUI_COLOR_FONT, this, &Default::GetColorFont));
}

// ----------------------------------------------------------------
// ------------------------ Layouts -------------------------------
// ----------------------------------------------------------------


bool Default::GetViewLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);
    // Vertical list alignment
    layout->attach(new Rect(ALIGNMENT, 0, 1, 0, 1));
    layout->get("Methods")->get("DrawFunc")
            ->Vertex::get(REQUEST)->get(GUI_DRAWER_LIST);

    layout->get("Fields")->get("ListElement")->get(ANY);

    Layout* controls = new Layout("Controls", FRAMEDLIST);
    // Horizontal list alignment
    controls->attach(new Rect(ALIGNMENT, 1, 0, 1, 0));
    layout->add(controls);

    controls->get("Fields")->get("ListElement")->get(BUTTON);
    return true;
}


bool Default::GetListLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);

    layout->attach(new Rect(ALIGNMENT, 0, 1, 0, 1));
    layout->get("Methods")->get("DrawFunc")
            ->Vertex::get(REQUEST)->get(GUI_DRAWER_LIST);

    Vertex* content = layout->get("Fields")->get("ListElement");
    content->get(BUTTON);
    content->get(TEXT);
    content->get(LIST);

    return true;
}


bool Default::GetFramedListLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);

    layout->set(new Rect(SIZEANDPOSITION, 0.1, 0.1, 0.8, 0.8));

    Layout* frame = new Layout("Frame", FRAME);
    layout->add(frame);

    Layout* content = new Layout("Content", LIST);
    layout->add(content);

    frame->get("Update")->attach(content);

    return true;
}


bool Default::GetButtonLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);

    layout->get("Methods")->get("DrawFunc")
            ->Vertex::get(REQUEST)->get(GUI_DRAWER_BUTTON);
    layout->set(new Rect(SIZEANDPOSITION, 0.1, 0.1, 0.8, 0.8));

    Layout* frame = new Layout("Frame", FRAME);
    layout->add(frame);

    // The Button container
    Layout* content = new Layout("Button Content", LIST);
    content->attach(new Rect(ALIGNMENT, 0, 1, 0, 1));
    layout->add(content);
    frame->get("Update")->attach(content);

    Layout* upper = new Layout("Upper", LIST);
    upper->attach(new Rect(ALIGNMENT, 1, 0, 0.5, 0));
    content->add(upper);

    Layout* lower = new Layout("Lower", LIST);
    lower->attach(new Rect(ALIGNMENT, 1, 0, 0.5, 0));
    content->add(lower);

    // Store the layer/layout types for the fields as simple nodes
    upper->get("Fields")->get(BTN_FIELD_ICON)->get(TEXT);
    upper->get("Fields")->get(BTN_FIELD_NAME)->get(TEXT);
    lower->get("Fields")->get(BTN_FIELD_DESCRIPTION)->get(TEXT);
    lower->get("Fields")->get(BTN_FIELD_CONTROL)->get(TEXT);

    return true;
}


bool Default::GetControlLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);

    Layout* frame = new Layout("Frame", FRAME);
    layout->add(frame);
    return true;
}


bool Default::GetFrameLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);

    layout->attach(new Rect(SIZEANDPOSITION, 0.01, 0.03, 0.98, 0.94));
    layout->get("Methods")->get("DrawFunc")
            ->Vertex::get(REQUEST)->get(GUI_DRAWER_FRAME);
    layout->get("Color")->Vertex::get(REQUEST)->get(GUI_COLOR_FRAME);

    Layout* bgrd = new Layout("Background", BACKGROUND);
    layout->add(bgrd);
    layout->get("Update")->attach(bgrd);

    return true;
}


bool Default::GetBackgroundLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);
    layout->name("Background Layout");

    layout->get("Methods")->get("DrawFunc")
            ->Vertex::get(REQUEST)->get(GUI_DRAWER_BACKGROUND);
    layout->get("Color")->Vertex::get(REQUEST)->get(GUI_COLOR_BACKGR_LIST);

    return true;
}


bool Default::GetTextLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);
    layout->name("Text Layout");

    layout->attach(new Rect(SIZEANDPOSITION, 0.1, 0.1, 0.8, 0.8));
    layout->get("Methods")->get("DrawFunc")
            ->Vertex::get(REQUEST)->get(GUI_DRAWER_TEXT);
    layout->get("Color")->Vertex::get(REQUEST)->get(GUI_COLOR_FONT);

    return true;
}


// ----------------------------------------------------------------
// ------------------------- Values -------------------------------
// ----------------------------------------------------------------


bool Default::GetColorFrame(Vertex* out)
{
    return out->attach(new Rgb("FrameColor", 0, 0, 200));
}

bool Default::GetColorBgrdList(Vertex* out)
{
    return out->attach(new Rgb("BackgroundColor", 20, 20, 50));
}

bool Default::GetColorBgrdButton(Vertex* out)
{
    return out->attach(new Rgb("BackgroundColor", 40, 40, 40));
}

bool Default::GetColorFont(Vertex* out)
{
    return out->attach(new Rgb("FontColor", 200, 200, 200));
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
    Rect* sap = GetRect(SIZEANDPOSITION, layout);
    sap->MultiplyTo(content_size);

    Vertex* children = layout->get("Update");
    if(children)
    {
        Vertex* to_update;
        uint i = 0;
        while((to_update=children->get(++i)) != NULL)
            GetRect(SIZEANDPOSITION, to_update)->Multiply(sap);
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

/*    if(!drawing || !drawing->is(GUI_DRAWER_VIEW) ||
            !drawing->is(VIRTUALSURFACE))
        return false;

    // A View has three possible components, 1. the frame
    Vertex* sub = drawing->get(LAYOUT_COMPONENT_FRAME);
    if(sub && sub->is(VIRTUALSURFACE))
        ((VirtualSurface*)sub)->Draw(true);
    // 2. the background
    sub = drawing->get(LAYOUT_COMPONENT_FRAME);
    if(sub && sub->is(VIRTUALSURFACE))
        ((VirtualSurface*)sub)->Draw(true);
    // 3. the content
    sub = drawing->get(LAYOUT_COMPONENT_CONTENT);
    if(sub && sub->is(VIRTUALSURFACE))
        ((VirtualSurface*)sub)->Draw(true);*/
    return true;
}


bool Default::DrawText(Vertex* layout)
{
    VirtualSurface* vs = GetSurface(layout);
    if(!vs)
        return false;

    string text = GetString(vs);
    if(text.empty())
        return false;

    SDL_Rect size = vs->GetSize();
    // Calculate the fitting font size
    int w, h;
    int fh = size.h*0.7;
    TTF_SizeText(GetFont(fh), text.c_str(), &w, &h);
    if((w>size.w) || (h>size.h))
        fh = h*double(size.w)*0.7/w;

    SDL_Surface* source = RenderText(&text, fh, GetRgb("FontColor", layout));

    Rect sub;
    PlaceCentered(source, size, sub);
    GetRect(SIZEANDPOSITION, layout)->Multiply(&sub);

    vs->SetBuffer(source);
    return true;
}


bool Default::DrawList(Vertex* layout)
{
    Vertex* children = layout->get("Update");
    if(!children)
        return false;

    uint child_cnt = children->size();
    if(child_cnt < 1)
        return true;

    Rect* align = GetRect(ALIGNMENT, layout);
    Rect* size = GetRect(SIZEANDPOSITION, layout);
    Rect calc;

    for(uint i=0; i<child_cnt; i++)
    {
        double c = double(child_cnt-i-1)/double(child_cnt-i);
        calc.w = (1 - (c * align->w))*(1 - calc.x);
        calc.h = (1 - (c * align->h))*(1 - calc.y);

        Rect* sub = GetRect(SIZEANDPOSITION, children->get(i+1));
        sub->Multiply(size);
        sub->Multiply(&calc);
        // Set the coordinates for the next iteration
        calc.x += (calc.w*align->x);
        calc.y += (calc.h*align->y);
    }

    return true;
}


bool Default::DrawButton(Vertex* layout)
{
    VirtualSurface* vs = GetSurface(layout);
    if(!vs)
        return false;

    return false;
}


bool Default::ColorSurface(Vertex* layout)
{
    VirtualSurface* vs = GetSurface(layout);
    if(!vs)
        return false;

    SDL_Surface* sf = vs->GetBuffer();
    SDL_Rect size = vs->GetSize();

    Rect* csap = GetRect(SIZEANDPOSITION, layout);
    csap->MultiplyTo(size);
    SDL_SetClipRect(sf, &size);
    FillRect(sf, &size, GetRgb("BackgroundColor", layout));

    return true;
}


string Default::GetString(Vertex* vs)
{
    Vertex* content = vs->get("Content")->get();
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
