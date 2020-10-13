#include "basicsdl1.h"
#include "defines.h"
#include "view/layer.h"
#include <iostream>


extern "C" Module* Create()
{
    return new BasicSdl1();
}


extern "C" void Destroy(Theme* theme)
{
    delete theme;
}



BasicSdl1::BasicSdl1()
    : m_Hmi("BasicSdl1", "Basic SDL1 visual theme"),
      m_Buttons("Buttons", "Customize"),
      m_Datas("Datas", "Customize"),
      m_Lists("Lists", "Customize"),
      m_Texts("Text fields", "Customize"),
      m_Views("Views", "Customize")
{
    // Properties
    m_Buttons.Add(new Rgb(BACKGROUNDCOLOR, "", 40, 40, 100));
    m_Buttons.Add(new Rgb(FRAMECOLOR, "", 30, 30, 75));
    m_Buttons.Add(new Rect(FRAMESIZE, "Relative frame size [%]", .5, .5, 1.0, 1.0));

    m_Datas.Add(new Rgb(BACKGROUNDCOLOR, "", 40, 40, 100));
    m_Datas.Add(new Rgb(FRAMECOLOR, "", 30, 30, 75));
    m_Datas.Add(new Rect(FRAMESIZE, "Relative frame size [%]", .5, .5, 1.0, 1.0));
    m_Datas.Add(new TData<bool>(DRAWFRAME, "Draw a visible frame around item", true));

    m_Lists.Add(new Rgb(BACKGROUNDCOLOR, "", 20, 20, 50));
    m_Lists.Add(new Rgb(FRAMECOLOR, "", 80, 80, 105));
    m_Lists.Add(new Rect(FRAMESIZE, "Relative frame size [%]", .5, .5, 1.0, 1.0));
    m_Lists.Add(new TData<bool>(DRAWFRAME, "Draw a visible frame around item", true));

    m_Texts.Add(new Rgb(BACKGROUNDCOLOR, "", 1, 1, 1));
    m_Texts.Add(new Rgb(FRAMECOLOR, "", 30, 30, 75));
    m_Texts.Add(new Rect(FRAMESIZE, "Relative frame size [%]", .5, .5, 1.0, 1.0));
    m_Texts.Add(new Rgb(FONTCOLOR, "", 200, 200, 200));
    m_Texts.Add(new TData<unsigned>(FONTSIZE, "", 20));

    m_Views.Add(new Rgb(BACKGROUNDCOLOR, "", 30, 30, 60));
    m_Views.Add(new Rgb(FRAMECOLOR, "", 30, 30, 75));
    m_Views.Add(new Rect(FRAMESIZE, "Relative frame size [%]", .5, .5, 1.0, 1.0));
    m_Views.Add(new TData<bool>(DRAWFRAME, "Draw a visible frame around item", true));

    m_Hmi.Attach(&m_Buttons);
    m_Hmi.Attach(&m_Lists);
    m_Hmi.Attach(&m_Texts);
    m_Hmi.Attach(&m_Views);
}


BasicSdl1::~BasicSdl1()
{
    for (auto curr = Fonts.begin(); curr != Fonts.end(); ++curr)
        TTF_CloseFont((*curr).second);

    // TODO: what if multiple SDL themes are loaded?
    atexit(SDL_Quit);
}


TTF_Font* BasicSdl1::GetFont(int size)
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


unsigned BasicSdl1::GetBaseSize()
{
    return ((TData<unsigned>*)(m_Texts.GetChild(FONTSIZE)))->GetValue();
}


void BasicSdl1::ToggleFullscreen(HmiItem*)
{
    m_IsFullscreen ? SetWindowed() : SetFullscreen();
}


void BasicSdl1::SetFullscreen()
{
    // This is the only way to get the HW screen resolution
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    m_Surface = SDL_SetVideoMode(
            info->current_w, info->current_h, 32,
            SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_FULLSCREEN);

    m_IsFullscreen = true;
}


void BasicSdl1::SetWindowed()
{
    m_Surface = SDL_SetVideoMode(1280, 1024, 32, SDL_DOUBLEBUF | SDL_HWSURFACE);
    m_IsFullscreen = false;
}


SDL_Rect BasicSdl1::GetResolution()
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


void BasicSdl1::InitScreen(Layer* root)
{
    m_ScreenRoot = root;
    DrawerSdl::GetDrawer(m_ScreenRoot)->RemoveFrame();
    // Start SDL as the default drawing engine:
    // Initialize the SDL library:
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
    {
        std::cout << SDL_GetError() << std::endl;
        exit(21);
    }
    SetWindowed();

    if (TTF_Init() == -1)
    {
        std::cout << TTF_GetError() << std::endl;
        exit(22);
    }

    // Add a func to toggle fullscreen <-> windowed mode
    // TODO
//    new Action<Screen>("Toggle full screen", "", this, &Screen::ToggleFullscreen);
}


void BasicSdl1::UpdateScreen()
{
    if (m_ScreenRoot->IsModified())
    {
        SDL_Rect tgt = GetResolution();
        SDL_Rect src = m_ScreenRoot->UpdateSize(tgt);
        // Clear screen.
        SDL_SetClipRect(m_Surface, &tgt);
        SDL_FillRect(m_Surface, nullptr, 0x000000);

        DrawerSdl::PlaceCentered(src, tgt);

        m_ScreenRoot->Draw(m_Surface);
        SDL_Flip(m_Surface);
    }
}
