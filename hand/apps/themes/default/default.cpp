#include "default.h"
#include "drawers.h"
#include "defines.h"
#include "virtualsurface.h"
#include "view/layer/layer.h"
#include <iostream>


extern "C" Module* Create()
{
    return (new Default());
}


extern "C" void Destroy(Theme* theme)
{
    delete theme;
}



Default::Default()
    : m_Hmi("Default", "Default visual theme"),
      m_Buttons("Buttons", "Customize"),
      m_Lists("Lists", "Customize"),
      m_Texts("Text fields", "Customize"),
      m_Views("Views", "Customize")
{
    // Properties
    m_Buttons.Add(new Rgb(BACKGROUNDCOLOR, "", 40, 40, 100));
    m_Buttons.Add(new Rgb(FRAMECOLOR, "", 30, 30, 75));
    m_Buttons.Add(new Rect(FRAMESIZE, "Relative frame size [%]", .01, .03, .98, .94));

    m_Lists.Add(new Rgb(BACKGROUNDCOLOR, "", 20, 20, 50));
    m_Lists.Add(new Rgb(FRAMECOLOR, "", 30, 30, 75));
    m_Lists.Add(new Rect(FRAMESIZE, "Relative frame size [%]", .01, .03, .98, .94));

    m_Texts.Add(new Rgb(BACKGROUNDCOLOR, "", 40, 40, 100));
    m_Texts.Add(new Rgb(FRAMECOLOR, "", 30, 30, 75));
    m_Texts.Add(new Rect(FRAMESIZE, "Relative frame size [%]", .01, .03, .98, .94));
    m_Texts.Add(new Rgb(FONTCOLOR, "", 200, 200, 200));

    m_Views.Add(new Rgb(BACKGROUNDCOLOR, "", 20, 20, 50));
    m_Views.Add(new Rgb(FRAMECOLOR, "", 30, 30, 75));
    m_Views.Add(new Rect(FRAMESIZE, "Relative frame size [%]", .01, .03, .98, .94));

    m_Hmi.Attach(&m_Buttons);
    m_Hmi.Attach(&m_Lists);
    m_Hmi.Attach(&m_Texts);
    m_Hmi.Attach(&m_Views);
}


Default::~Default()
{
    for (auto curr = Fonts.begin(); curr != Fonts.end(); ++curr)
        TTF_CloseFont((*curr).second);

    // TODO: what if multiple SDL themes are loaded?
    atexit(SDL_Quit);
}


TTF_Font* Default::GetFont(int size)
{
    // Loading font:
    TTF_Font* font = Fonts[size];
    if(!font)
    {
        font = TTF_OpenFont(FONT_FILE, size);
        if(!font)
        {
            std::cout << "Unable to load font:" << FONT_FILE
                    << "\nTTF_OpenFont: " << TTF_GetError() << std::endl;
            exit(25);
        }
        Fonts[size] = font;
    }
    return font;
}

/*
void Default::GetFontHeight(HmiItem* layout, unsigned& max_size)
{
    HmiItem* pref = layout->get(DIM_FONT_PREFERRED);
    if (pref)
        max_size = ((Data<unsigned>*)pref)->get();
    else
    {
        unsigned max = ((Data<unsigned>*)layout->get(DIM_FONT_MAX))->get();
        if(max_size >= max)
            max_size = max;
    }
}*/


void Default::ToggleFullscreen(HmiItem*)
{
    m_IsFullscreen ? SetWindowed() : SetFullscreen();
}


void Default::SetFullscreen()
{
    // This is the only way to get the HW screen resolution
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    m_Surface = SDL_SetVideoMode(
            info->current_w, info->current_h, 32,
            SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_FULLSCREEN);
    UpdateRoot();

    m_IsFullscreen = true;
}


void Default::SetWindowed()
{
    m_Surface = SDL_SetVideoMode(1280, 1024, 32, SDL_DOUBLEBUF | SDL_HWSURFACE);
    UpdateRoot();

    m_IsFullscreen = false;
}


void Default::UpdateRoot()
{
    if (!m_Surface)
    {
        std::cout << SDL_GetError() << std::endl;
        exit(23);
    }

    m_ScreenRoot->SetSize(GetResolution());
    VirtualSurface::GetDrawer(m_ScreenRoot)->SetBuffer(m_Surface);
}


SDL_Rect Default::GetResolution()
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


void Default::InitScreen(Layer* a_root)
{
    m_ScreenRoot = a_root;
    VirtualSurface::GetDrawer(m_ScreenRoot)->RemoveFrame();
    // Start SDL as the default drawing engine:
    // Initialize the SDL library:
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
    {
        std::cout << SDL_GetError() << std::endl;
        exit(21);
    }
    SetWindowed();

    // Add a func to toggle fullscreen <-> windowed mode
    // TODO
//    new Action<Screen>("Toggle full screen", "", this, &Screen::ToggleFullscreen);
    if (TTF_Init() == -1)
    {
        std::cout << TTF_GetError() << std::endl;
        exit(22);
    }
}


void Default::UpdateScreen()
{
    m_ScreenRoot->Draw(false);
    SDL_Flip(m_Surface);
}


Drawer* Default::GetButtonDrawer()
{
    VirtualSurface* ret = new ButtonDrawer(this);
    ret->SetProperties(&m_Buttons);
    return ret;
}


Drawer* Default::GetListDrawer()
{
    VirtualSurface* ret = new ListDrawer(this);
    ret->SetProperties(&m_Lists);
    return ret;
}


Drawer* Default::GetTextDrawer()
{
    VirtualSurface* ret = new TextDrawer(this);
    ret->SetProperties(&m_Texts);
    return ret;
}


Drawer* Default::GetViewDrawer()
{
    VirtualSurface* ret = new ViewDrawer(this);
    ret->SetProperties(&m_Views);
    return ret;
}
