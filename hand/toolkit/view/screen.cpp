#include "view/screen.h"
#include "view/layer/layer.h"
#include "graph/data.h"
#include "graph/method.h"
#include "base/modulelib.h"
#include "view/theme.h"
#include <iostream>


Screen::Screen() : m_View("Screen", ""), m_Users("Users", "Views"), m_Menu("Menu", "Settings")
{
    m_IsFullscreen = false;
    // Start SDL as the default drawing engine:
    InitSDL();
    SetWindowed();

    m_View.SetView(m_Users.GetLayer());
    m_View.SetControls(&m_Menu);

    m_ThemeLoader = new ModuleLib();
    m_Menu.Add(new Note(
            "Theme", "Select default visualization theme for all users.",
            "./binaries/lib/themes/default.so", m_ThemeLoader));

    // TODO: load settings
    m_View.GetLayer()->SetTheme(dynamic_cast<Theme*>(m_ThemeLoader->GetObject()));

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
    // TODO: generic way to pass size and position
    m_View.GetLayer()->SetSize(GetResolution());

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
    // TODO: generic way to pass size and position
    m_View.GetLayer()->SetSize(GetResolution());

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


void Screen::AddView(HmiItem* a_viewItem)
{
    m_Users.Attach(a_viewItem);
}


void Screen::RemoveView(HmiItem* a_viewItem)
{
    m_Users.Remove(a_viewItem);
}


bool Screen::ShowSurface()
{
    if (!m_Users.Size())
        return false;

    m_View.GetLayer()->Show(false);

    SDL_Flip(m_Surface);
    return true;
}
