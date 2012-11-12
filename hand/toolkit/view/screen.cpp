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
#include "view/screen.h"
#include "view/layer/layer.h"
#include "graph/data.h"
#include "graph/method.h"


using namespace std;


Screen::Screen() : HandApp("Screen")
{
    IsFullscreen = false;
    // Start SDL as the default drawing engine:
    InitSDL();
    SetWindowed();
}


void Screen::Init()
{
    // Add a func to toggle fullscreen <-> windowed mode
    add(new Method<Screen>("Toggle full screen", this, &Screen::ToggleFullscreen));
}


bool Screen::ToggleFullscreen(Vertex* ignore)
{
    bool ret = IsFullscreen ? SetWindowed() : SetFullscreen();
    return ret;
}


void Screen::InitSDL(void)
{
    atexit(SDL_Quit);
    // Initialize the SDL library:
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
    {
        cout << SDL_GetError() << endl;
        exit(21);
    }
}


bool Screen::SetFullscreen()
{
    // This is the only chance to get the HW screen resolution
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    Surface = SDL_SetVideoMode(info->current_w, info->current_h, 32,
                               SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_FULLSCREEN);
    if(Surface == NULL)
    {
        cout << SDL_GetError() << endl;
        exit(23);
    }
    return (IsFullscreen = true);
}


bool Screen::SetWindowed()
{
    Surface = SDL_SetVideoMode(1280, 1024, 32,
                               SDL_DOUBLEBUF|SDL_HWSURFACE);
    if(!Surface)
    {
        cout << SDL_GetError() << endl;
        exit(24);
    }
    return (IsFullscreen = false);
}


SDL_Rect Screen::GetResolution()
{
    const SDL_VideoInfo* inf;
    // Return real application size
    Uint16 biggest_Uint16 = 65535;
    inf = SDL_GetVideoInfo();
    SDL_Rect tmp = {0, 0, 0, 0};
    if(inf->current_w <= biggest_Uint16)
        tmp.w = inf->current_w;
    else
        tmp.w = biggest_Uint16;
    if(inf->current_h <= biggest_Uint16)
        tmp.h = inf->current_h;
    else
        tmp.h = biggest_Uint16;
    return tmp;
}


bool Screen::SetLayerManagerPositions()
{
    Vertex* all_lm = Vertex::get(LAYERMANAGER);
    if(all_lm->size() < 1)
        return false;

    SDL_Rect screen = GetResolution();
    SDL_Rect screen_tmp = screen;

    Layer* lm;
    uint i = 0;
    while((lm=dynamic_cast<Layer*>(all_lm->get(++i))) != NULL)
    {
        screen_tmp.w = screen.w/all_lm->size();
        screen_tmp.x = screen_tmp.w*i;
        lm->SetSize(screen_tmp);
    }
    return true;
}


SDL_Surface* Screen::GetSurface()
{
    return Surface;
}


bool Screen::ShowSurface(void)
{
    if(!SetLayerManagerPositions())
        return false;

    Vertex* all_lm = Vertex::get(LAYERMANAGER);
    Layer* layer;
    uint i = 0;
    while((layer=dynamic_cast<Layer*>(all_lm->get(++i))) != NULL)
        layer->Update(false);

    // Need to call this extra as BuildSurface is called recursively
    SDL_Flip(Surface);
    return true;
}
