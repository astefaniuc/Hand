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

#include "view/theme/theme.h"
#include "view/datatypes/rgb.h"
#include "view/datatypes/rect.h"
#include "view/datatypes/layout.h"
#include "view/layer/virtualsurface.h"

#include <iostream>
#include <dlfcn.h>


using namespace std;


Theme::Theme(string name) : HandApp(name)
{
    // This opens the value for the main application TODO: needed?
    dlopen(NULL, RTLD_LAZY | RTLD_GLOBAL);
    if(TTF_Init() == -1)
    {
        cout << TTF_GetError() << endl;
        exit(22);
    }
    Map = new FactoryMap("Map");
}


Theme::~Theme()
{
    map<int, TTF_Font*>::iterator curr = Fonts.begin();
    while(curr!=Fonts.end())
    {
        TTF_CloseFont((*curr).second);
        curr++;
    }
    delete(Map);
}


void Theme::Register(Factory* f)
{
    Map->Add(f);
}


bool Theme::Execute(Functoid* request)
{
    Map->Produce(request, request->GetType());
    return FillOut(request);
}


bool Theme::FillOut(Functoid* input)
{
    if(!input)
        return false;
    Layout* layout = dynamic_cast<Layout*>(input);
    if(!layout)
        return true;

    string name;
    FunctoidIterator _end = layout->end();
    for(FunctoidIterator curr=layout->begin(); curr!=_end; curr++)
    {
        name = (*curr)->GetName();
        if((name==CHILDREN) || (name=="Methods"))
            _FillOut((*curr));
        else if((name=="Fields") || (name=="Components"))
            continue;
        else
            Execute((*curr));
    }
    return true;
}


bool Theme::_FillOut(Functoid* input)
{
    FunctoidIterator _end = input->end();
    for(FunctoidIterator curr=input->begin(); curr!=_end; curr++)
        Execute((*curr));

    return true;
}


TTF_Font* Theme::GetFont(int size)
{
    // Loading font:
    TTF_Font* font = Fonts[size];
    if(font == NULL)
    {
        font = TTF_OpenFont(FONT_FILE, size);
        if(font == NULL)
        {
            cout << "Unable to load font:" << FONT_FILE << endl;
            cout << "TTF_OpenFont: " << TTF_GetError() << endl;
            cout.flush();
            exit(25);
        }
        Fonts[size] = font;
    }
    return font;
}


void Theme::GetFontHeight(Functoid* layout, uint& max_size)
{
    Functoid* pref = layout->Get(DIM_FONT_PREFERRED);
    if(pref)
        max_size = ((Data<uint>*)pref)->Get();
    else
    {
        uint max = ((Data<uint>*)layout->Get(DIM_FONT_MAX))->Get();
        if(max_size >= max)
            max_size = max;
    }
}


VirtualSurface* Theme::GetSurface(Functoid* in)
{
    Layout* layout = dynamic_cast<Layout*>(in);
    if(layout)
        return dynamic_cast<VirtualSurface*>(layout->Get("Surface")->Get(1));
    return NULL;
}


void Theme::FillRect(SDL_Surface* sf, SDL_Rect* r, Rgb* c)
{
    SDL_FillRect(sf, r, SDL_MapRGB(sf->format, c->r, c->g, c->b));
}


SDL_Surface* Theme::RenderText(string* text, int size, Rgb* color)
{
    SDL_Color sdl_color;
    sdl_color.r = color->r;
    sdl_color.g = color->g;
    sdl_color.b = color->b;
    SDL_Surface* sf = TTF_RenderText_Blended(GetFont(size),
                                             text->c_str(),
                                             sdl_color);
    return sf;
}


void PlaceCentered(SDL_Surface* source, SDL_Rect& target, Rect& out)
{
    out.w = double(source->w)/double(target.w);
    out.h = double(source->h)/double(target.h);
    out.x = (1 - out.w)/2;
    out.y = (1 - out.h)/2;
}

/*
void SqueezeRect(SDL_Rect& rect, Rect factor)
{
    SDL_Rect tmp = rect;
    rect.w  = rect.w*factor.w;
    rect.h  = rect.h*factor.h;
    PlaceCentered(rect, tmp);
}


void SqueezeRect(SDL_Rect& rect, double factor)
{
    SDL_Rect tmp = rect;
    rect.w  = rect.w*factor/100;
    rect.h  = rect.h*factor/100;
    PlaceCentered(rect, tmp);
}
*/
