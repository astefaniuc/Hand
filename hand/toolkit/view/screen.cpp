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

#include "view/screen.h"
#include "base/handserver.h"

#include <iostream>


using namespace std;


Screen::Screen() : HandApp("Screen")
{
    IsFullscreen = false;
    // Start SDL as the default drawing engine:
    InitSDL();
    // This is the only chance to get the HW screen resolution
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    FullSize.w = info->current_w;
    FullSize.h = info->current_h;
    // Set the default resolution
    WndSize.w = 800;
    WndSize.h = 600;
    SetWindowed();
}


void Screen::Init()
{
    // Add a func to toggle fullscreen <-> windowed mode
    List* button = new List("ToggleFullscreen");
    button->Add(new Method<Screen>("Button", this, &Screen::ToggleFullscreen));
    button->Add(new Note(TAG_DESCRIPTION, "Toggle full screen"));
    Add(button);
}


bool Screen::ToggleFullscreen(Vertex* ignore)
{
    bool ret = IsFullscreen ? SetWindowed() : SetFullscreen();
    Server()->SetLayerManagerPositions();
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


void Screen::SetSize(Size* res)
{
    WndSize.h = res->h;
    WndSize.w = res->w;
    // Minimal size
    if(WndSize.h <= 200) WndSize.h = 200;
    if(WndSize.w <= 200) WndSize.w = 200;
}


bool Screen::SetFullscreen()
{
    Surface = SDL_SetVideoMode(FullSize.w, FullSize.h, 32,
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
    Surface = SDL_SetVideoMode(WndSize.w, WndSize.h, 32,
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


SDL_Surface* Screen::GetSurface()
{
    return Surface;
}


void Screen::ShowSurface(void)
{
    // Need to call this extra as BuildSurface is called recursively
    SDL_Flip(Surface);
}
