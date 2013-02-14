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

#include <iostream>
#include "view/theme/theme.h"
#include "view/datatypes/rgb.h"
#include "view/datatypes/rect.h"
#include "view/datatypes/layout.h"
#include "view/layer/virtualsurface.h"


using namespace std;


Theme::Theme(string name) : HandApp(name)
{
    if(TTF_Init() == -1)
    {
        cout << TTF_GetError() << endl;
        exit(22);
    }
    type(THEME);
}


Theme::~Theme()
{
    map<int, TTF_Font*>::iterator curr = Fonts.begin();
    while(curr!=Fonts.end())
    {
        TTF_CloseFont((*curr).second);
        curr++;
    }
}


bool Theme::execute(Vertex* input)
{
    if(!dynamic_cast<Layout*>(input))
        return false;

    // Get the layout factory
    Vertex* req = input->Vertex::get(ANY, REQUEST);
    if(req)
    {
        req = req->get();
        if(req)
            get(LAYOUT)->get(req->name())->execute(input);
    }

    string name;
    Vertex* sub;
    uint i = 0;
    while((sub=input->get(++i)) != NULL)
    {
        name = sub->name();
        if((name==LAYER_FACTORIES) || (name==TARGET) || (name==THEME))
            continue;

        if(name == FIELDS)
        {
            uint j = 0;
            Vertex* child;
            while((child=sub->get(++j)) != NULL)
                execute(child);
        }
        else
            FillOut(sub);
    }
    return true;
}


bool Theme::FillOut(Vertex* request)
{
    Vertex* req = request->Vertex::get(ANY, REQUEST);
    if(req)
        req = req->get();
    if(!req)
        // Nothing to do
        return false;
    if(request->get(req->name(), ANY))
        // Already resolved
        return false;
    // TODO: Vertex::get(Vertex* path)
    Vertex* repo = get(ANY, req->name());
    if(!repo)
        return false;
    while((req=req->get()) != NULL)
    {
        repo = repo->get(ANY, req->name());
        if(!repo)
            return false;
    }
    if(repo->is(FACTORY))
        return request->set(repo->get());
    return request->set(repo);
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


void Theme::GetFontHeight(Vertex* layout, uint& max_size)
{
    Vertex* pref = layout->get(DIM_FONT_PREFERRED);
    if(pref)
        max_size = ((Data<uint>*)pref)->get();
    else
    {
        uint max = ((Data<uint>*)layout->get(DIM_FONT_MAX))->get();
        if(max_size >= max)
            max_size = max;
    }
}


VirtualSurface* Theme::GetSurface(Vertex* in)
{
    return dynamic_cast<VirtualSurface*>(in->get(LAYER, ANY));
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


void Theme::PlaceCentered(SDL_Surface* source, SDL_Rect& target, Rel_Rect& out)
{
    out.w = double(source->w)/double(target.w);
    out.h = double(source->h)/double(target.h);
    out.x = (1 - out.w)/2;
    out.y = (1 - out.h)/2;
}
