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
#include "view/datatypes/rect.h"
#include "view/datatypes/rgb.h"
#include "view/datatypes/layout.h"
#include "view/theme/drawer.h"
#include "view/layer/virtualsurface.h"


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
    Register(new LayoutFactory<Default>(GUI_LAYOUT_LAYERMANAGER, this, &Default::GetLMLayout));
    Register(new LayoutFactory<Default>(GUI_LAYOUT_VIEW, this, &Default::GetViewLayout));
    Register(new LayoutFactory<Default>(GUI_LAYOUT_FRAMEDLIST, this, &Default::GetFramedListLayout));
    Register(new LayoutFactory<Default>(GUI_LAYOUT_LIST, this, &Default::GetListLayout));
    Register(new LayoutFactory<Default>(GUI_LAYOUT_CONTROLID, this, &Default::GetControlLayout));
    Register(new LayoutFactory<Default>(GUI_LAYOUT_BUTTON, this, &Default::GetButtonLayout));
    Register(new LayoutFactory<Default>(GUI_LAYOUT_FRAME, this, &Default::GetFrameLayout));
    Register(new LayoutFactory<Default>(GUI_LAYOUT_BACKGROUND, this, &Default::GetBackgroundLayout));
    Register(new LayoutFactory<Default>(GUI_LAYOUT_TEXT, this, &Default::GetTextLayout));

    // Colors
    Register(new PropertyFactory<Default>(GUI_COLOR_FRAME, this, &Default::GetColorFrame));
    Register(new PropertyFactory<Default>(GUI_COLOR_BACKGR_LIST, this, &Default::GetColorBgrdList));
    Register(new PropertyFactory<Default>(GUI_COLOR_BACKGR_BUTTON, this, &Default::GetColorBgrdButton));
    Register(new PropertyFactory<Default>(GUI_COLOR_FONT, this, &Default::GetColorFont));
}

// ----------------------------------------------------------------
// ------------------------ Layouts -------------------------------
// ----------------------------------------------------------------


bool Default::GetLMLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);

    layout->AddField("MasterLayer", "Any");
    return true;
}


bool Default::GetViewLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);
    // Vertical list alignment
    layout->Set(new Rect(ALIGNMENT, 0, 1, 0, 1));
    layout->Get("Methods")->Add(new Link("DrawFunc", GUI_DRAWER_LIST));
    layout->AddField("ListElement", "Any");

    Layout* controls = new Layout("Controls", GUI_LAYOUT_FRAMEDLIST);
    // Horizontal list alignment
    controls->Set(new Rect(ALIGNMENT, 1, 0, 1, 0));
    layout->Get(CHILDREN)->Add(controls);
    controls->AddField("ListElement", BUTTONLAYER);
    return true;
}


bool Default::GetListLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);

    layout->Set(new Rect(ALIGNMENT, 0, 1, 0, 1));
    layout->Get("Methods")->Add(new Link("DrawFunc", GUI_DRAWER_LIST));
    layout->AddField("ListElement", BUTTONLAYER);
//    layout->AddField("ListElement", "Any");
    return true;
}


bool Default::GetFramedListLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);

    layout->Set(new Rect(SIZEANDPOSITION, 0.1, 0.1, 0.8, 0.8));

    Layout* frame = new Layout("Background", GUI_LAYOUT_BACKGROUND);
    layout->Get(CHILDREN)->Add(frame);

    Layout* content = new Layout("Content", GUI_LAYOUT_LIST);
    layout->Get(CHILDREN)->Add(content);
    frame->Get("Update")->Attach(content);

    return true;
}


bool Default::GetButtonLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);

    layout->Get("Methods")->Add(new Link("DrawFunc", GUI_DRAWER_BUTTON));
    layout->Set(new Rect(SIZEANDPOSITION, 0.1, 0.1, 0.8, 0.8));

    Layout* frame = new Layout("Frame", GUI_LAYOUT_FRAME);
    layout->Get(CHILDREN)->Add(frame);
    // The Button container
    Layout* content = new Layout("Button Content", GUI_LAYOUT_LIST);
    content->Set(new Rect(ALIGNMENT, 0, 1, 0, 1));
    layout->Get(CHILDREN)->Add(content);
    frame->Get("Update")->Attach(content);

    Layout* upper = new Layout("Upper", GUI_LAYOUT_LIST);
    upper->Set(new Rect(ALIGNMENT, 1, 0, 0.5, 0));
    upper->AddField(BTN_FIELD_ICON, TEXTLAYER);
    upper->AddField(BTN_FIELD_NAME, TEXTLAYER);
    content->Get(CHILDREN)->Add(upper);

    Layout* lower = new Layout("Lower", GUI_LAYOUT_LIST);
    lower->Set(new Rect(ALIGNMENT, 1, 0, 0.5, 0));
    lower->AddField(BTN_FIELD_DESCRIPTION, TEXTLAYER);
    lower->AddField(BTN_FIELD_CONTROL, BUTTONLAYER);
    content->Get(CHILDREN)->Add(lower);

    return true;
}


bool Default::GetControlLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);

    Layout* frame = new Layout("Frame", GUI_LAYOUT_FRAME);
    layout->Get(CHILDREN)->Add(frame);
    return true;
}


bool Default::GetFrameLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);

    layout->Set(new Rect(SIZEANDPOSITION, 0.01, 0.03, 0.98, 0.94));
    layout->Set(new Link("Color", GUI_COLOR_FRAME));
    layout->Get("Methods")->Add(new Link("DrawFunc", GUI_DRAWER_FRAME));

    Layout* bgrd = new Layout("Background", GUI_LAYOUT_BACKGROUND);
    layout->Get(CHILDREN)->Add(bgrd);
    layout->Get("Update")->Attach(bgrd);

    return true;
}


bool Default::GetBackgroundLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);
    layout->SetName("Background Layout");

    layout->Get("Methods")->Add(new Link("DrawFunc", GUI_DRAWER_BACKGROUND));
    layout->Set(new Link("Color", GUI_COLOR_BACKGR_LIST));
    return true;
}


bool Default::GetTextLayout(Vertex* out)
{
    Layout* layout = dynamic_cast<Layout*>(out);
    layout->SetName("Text Layout");

    layout->Set(new Rect(SIZEANDPOSITION, 0.1, 0.1, 0.8, 0.8));
    layout->Get("Methods")->Add(new Link("DrawFunc", GUI_DRAWER_TEXT));
    layout->Set(new Link("Color", GUI_COLOR_FONT));

/*    Dim_Font_Preferred = 32;
    Dim_Font_Min = 8;
    Color_Alt_Font.Set(0, 0, 0);*/
    return true;
}


// ----------------------------------------------------------------
// ------------------------- Values -------------------------------
// ----------------------------------------------------------------


bool Default::GetColorFrame(Vertex* out)
{
    return out->Set(new Rgb("FrameColor", 0, 0, 200));
}

bool Default::GetColorBgrdList(Vertex* out)
{
    return out->Set(new Rgb("BackgroundColor", 20, 20, 50));
}

bool Default::GetColorBgrdButton(Vertex* out)
{
    return out->Set(new Rgb("BackgroundColor", 40, 40, 40));
}

bool Default::GetColorFont(Vertex* out)
{
    return out->Set(new Rgb("FontColor", 200, 200, 200));
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

    Vertex* children = layout->Get("Update");
    if(children)
    {
        Vertex* to_update;
        uint i = 0;
        while((to_update=children->Get(++i)) != NULL)
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

/*    if(!drawing || !drawing->IsType(GUI_DRAWER_VIEW) ||
            !drawing->IsType(VIRTUALSURFACE))
        return false;

    // A View has three possible components, 1. the frame
    Vertex* sub = drawing->Get(LAYOUT_COMPONENT_FRAME);
    if(sub && sub->IsType(VIRTUALSURFACE))
        ((VirtualSurface*)sub)->Draw(true);
    // 2. the background
    sub = drawing->Get(LAYOUT_COMPONENT_FRAME);
    if(sub && sub->IsType(VIRTUALSURFACE))
        ((VirtualSurface*)sub)->Draw(true);
    // 3. the content
    sub = drawing->Get(LAYOUT_COMPONENT_CONTENT);
    if(sub && sub->IsType(VIRTUALSURFACE))
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
    // Starting SDL font rendering:
    uint font_height = size.h;
    // Render the font once to check the size
    Rgb* font_color = GetRgb("FontColor", layout);
    SDL_Surface* source = RenderText(&text, font_height, font_color);
    if(source->w > size.w)
    {
        // Calculate new height and re-render
        font_height = (font_height*size.w)/source->w;
        SDL_FreeSurface(source);
        source = RenderText(&text, font_height, font_color);
    }
    Rect sub;
    PlaceCentered(source, size, sub);
    GetRect(SIZEANDPOSITION, layout)->Multiply(&sub);

    vs->SetBuffer(source);
    return true;
}


bool Default::DrawList(Vertex* layout)
{
    Vertex* children = layout->Get("Update");
    if(!children)
        return false;
    // Ignore the "runtime" child
    uint child_cnt = children->size() - 1;
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

        Rect* sub = GetRect(SIZEANDPOSITION, children->Get(i+1));
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
    Vertex* content = vs->Get("Content")->Get(1);
    if(content)
        return content->GetName();
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
