#include <iostream>
#include "view/screen.h"
#include "view/layer/layer.h"
#include "graph/data.h"
#include "graph/method.h"


Screen::Screen()
{
    m_IsFullscreen = false;
    // Start SDL as the default drawing engine:
    InitSDL();
    SetWindowed();

    // Add a func to toggle fullscreen <-> windowed mode
    // TODO
//    new Action<Screen>("Toggle full screen", "", this, &Screen::ToggleFullscreen);
}


void Screen::ToggleFullscreen(HmiItem*)
{
    m_IsFullscreen ? SetWindowed() : SetFullscreen();
}


void Screen::InitSDL()
{
    atexit(SDL_Quit);
    // Initialize the SDL library:
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
    {
        std::cout << SDL_GetError() << std::endl;
        exit(21);
    }
}


bool Screen::SetFullscreen()
{
    // This is the only chance to get the HW screen resolution
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    m_Surface = SDL_SetVideoMode(
            info->current_w, info->current_h, 32, SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_FULLSCREEN);
    if(!m_Surface)
    {
        std::cout << SDL_GetError() << std::endl;
        exit(23);
    }
    return (m_IsFullscreen = true);
}


bool Screen::SetWindowed()
{
    m_Surface = SDL_SetVideoMode(1280, 1024, 32, SDL_DOUBLEBUF|SDL_HWSURFACE);
    if(!m_Surface)
    {
        std::cout << SDL_GetError() << std::endl;
        exit(24);
    }
    return (m_IsFullscreen = false);
}


SDL_Rect Screen::GetResolution()
{
    const Uint16 maxUint16 = 65535;
    const SDL_VideoInfo* inf = SDL_GetVideoInfo();

    SDL_Rect tmp = { 0, 0, 0, 0 };
    if ((inf->current_w <= maxUint16) && (inf->current_h <= maxUint16))
    {
        tmp.w = inf->current_w;
        tmp.h = inf->current_h;
    }
    return tmp;
}


void Screen::Add(CUser* a_user)
{
    m_Users.push_back(a_user);
    SplitScreen();
}


void Screen::Remove(CUser* a_user)
{
    for (unsigned i = 0; i < m_Users.size(); ++i)
    {
        if (m_Users[i] == a_user)
        {
            m_Users.erase(m_Users.begin() + i);
            // Recalculate areas for remaining users
            SplitScreen();
            return;
        }
    }
}


void Screen::SplitScreen()
{
    if (!m_Users.size())
        return;

    SDL_Rect screen = GetResolution();
    Uint16 newWidth = screen.w / m_Users.size();

    for (unsigned i = 0; i < m_Users.size(); ++i)
    {
        screen.w = newWidth;
        screen.x = newWidth * (i - 1);
        m_Users[i]->SetSize(screen);
        // TODO: else, generic way to pass size and position
    }
}


bool Screen::ShowSurface()
{
    if (!m_Users.size())
        return false;

    for (CUser* user : m_Users)
        user->Update(false);

    // Need to call this extra as BuildSurface is called recursively
    SDL_Flip(m_Surface);
    return true;
}
