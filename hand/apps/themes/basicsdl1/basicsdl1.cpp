#include "basicsdl1.h"
#include "defines.h"
#include "data/interface.h"
#include "data/method.h"
#include "view/layer.h"
#include "view/layers/hmi/interface.h"
#include <iostream>


extern "C" Module* Create()
{
    return new BasicSdl1();
}


extern "C" void Destroy(Theme* theme)
{
    delete theme;
}



BasicSdl1::BasicSdl1() : m_Hmi("BasicSdl1", "Basic SDL1 visual theme")
{
    // Properties
    m_Buttons = new Hmi::Map("Datas", "Customize");
    m_Buttons->Add(new Rgb(BACKGROUNDCOLOR, "", 45, 45, 75));
    m_Buttons->Add(new Rgb(FRAMECOLOR, "", 75, 70, 130));
    m_Buttons->Add(new Rect(FRAMESIZE, "Relative frame size [%]", .5, .5, 0.7, 0.7));
    m_Buttons->Add(new Hmi::TData<bool>(DRAWFRAME, "Draw a visible frame around item", true));

    m_Datas = new Hmi::Map("Buttons", "Customize");
    m_Datas->Add(new Rgb(BACKGROUNDCOLOR, "", 40, 40, 85));
    m_Datas->Add(new Rgb(FRAMECOLOR, "", 65, 60, 150));
    m_Datas->Add(new Rect(FRAMESIZE, "Relative frame size [%]", .5, .5, 0.4, 0.4));
    m_Datas->Add(new Hmi::TData<bool>(DRAWFRAME, "Draw a visible frame around item", true));

    m_Chords = new Hmi::Map("Chords", "Customize");
    m_Chords->Add(new Rgb(BACKGROUNDCOLOR, "", 30, 75, 20));
    m_Chords->Add(new Rgb(FRAMECOLOR, "", 55, 105, 45));
    m_Chords->Add(new Rect(FRAMESIZE, "Relative frame size [%]", .5, .5, 0.4, 0.4));
    m_Chords->Add(new Hmi::TData<bool>(DRAWFRAME, "Draw a visible frame around item", true));

    m_Lists = new Hmi::Map("Lists", "Customize");
    m_Lists->Add(new Rgb(BACKGROUNDCOLOR, "", 0, 11, 11));
    m_Lists->Add(new Rgb(FRAMECOLOR, "", 2, 26, 26));
    m_Lists->Add(new Rect(FRAMESIZE, "Relative frame size [%]", .5, .5, 0.6, 0.6));
    m_Lists->Add(new Hmi::TData<bool>(DRAWFRAME, "Draw a visible frame around item", true));

    m_Texts = new Hmi::Map("Text fields", "Customize");
    m_Texts->Add(new Rgb(BACKGROUNDCOLOR, "", 1, 1, 1));
    m_Texts->Add(new Rgb(FRAMECOLOR, "", 30, 30, 75));
    m_Texts->Add(new Rect(FRAMESIZE, "Relative frame size [%]", .5, .5, 0.6, 0.6));
    m_Texts->Add(new Rgb(FONTCOLOR, "", 180, 180, 180));
    m_Texts->Add(new Hmi::TData<unsigned>(FONTSIZE, "", 16));

    m_Views = new Hmi::Map("Views", "Customize");
    m_Views->Add(new Rgb(BACKGROUNDCOLOR, "", 5, 0, 14));
    m_Views->Add(new Rgb(FRAMECOLOR, "", 11, 2, 41));
    m_Views->Add(new Rect(FRAMESIZE, "Relative frame size [%]", .5, .5, 1.0, 1.0));
    m_Views->Add(new Hmi::TData<bool>(DRAWFRAME, "Draw a visible frame around item", true));

    Hmi::Interface* settings = new Hmi::Interface("Settings", "");
    settings->AddControl(m_Buttons);
    settings->AddControl(m_Datas);
    settings->AddControl(m_Chords);
    settings->AddControl(m_Lists);
    settings->AddControl(m_Texts);
    settings->AddControl(m_Views);

    m_Hmi.Add(settings);
    m_Hmi.Add(new Hmi::Action<BasicSdl1>(
        "Toggle full screen", "", this, &BasicSdl1::ToggleFullscreen));
}


BasicSdl1::~BasicSdl1()
{
    for (auto it = Fonts.begin(); it != Fonts.end(); ++it)
        TTF_CloseFont((*it).second);

    for (DrawerSdl* d : m_Drawers)
        delete d;
    // TODO: we can't call atexit(SDL_Quit) here because it doesn' work from a lib
    // And what if multiple SDL themes are loaded?
}


void BasicSdl1::GetHmi(Layer* caller)
{
    caller->GetData()->GetInterface()->AttachControl(&m_Hmi);
}


TTF_Font* BasicSdl1::GetFont(int size)
{
    // Loading font:
    TTF_Font* font = Fonts[size];
    if (!font)
    {
        font = TTF_OpenFont(FONT_FILE, size);
        if (!font)
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
    return ((Hmi::TData<unsigned>*)(m_Texts->GetChild(FONTSIZE)))->GetValue();
}


void BasicSdl1::InitScreen(Layer* root)
{
    m_ScreenRoot = root;
    DrawerSdl::GetDrawer(root)->RemoveFrame();
    // Start SDL as the default drawing engine:
    // Initialize the SDL library:
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        std::cout << SDL_GetError() << std::endl;
        exit(21);
    }

    if (TTF_Init() == -1)
    {
        std::cout << TTF_GetError() << std::endl;
        exit(22);
    }

    DrawerSdl::GetDrawer(root)->RemoveFrame();

    m_Window = SDL_CreateWindow(
        "Hand", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 1024, SDL_WINDOW_RESIZABLE);
}


void BasicSdl1::UpdateScreen(bool forced)
{
    if (!forced && !m_ScreenRoot->IsModified())
        return;

    SDL_Rect res = GetResolution();
    ((Field::Item*)m_ScreenRoot)->ComputeSize(res);
    ((Field::Item*)m_ScreenRoot)->UpdatePositions(res);
    m_ScreenRoot->Draw(SDL_GetWindowSurface(m_Window));
    SDL_UpdateWindowSurface(m_Window);
}


void BasicSdl1::ToggleFullscreen(Layers::Item*)
{
    bool isFullscreen = SDL_GetWindowFlags(m_Window) & SDL_WINDOW_FULLSCREEN;
    SDL_SetWindowFullscreen(m_Window, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);

    m_ScreenRoot->SetModified(true);
}


SDL_Rect BasicSdl1::GetResolution()
{
    SDL_Rect tmp = { 0, 0, 0, 0 };
    SDL_GetWindowSize(m_Window, &tmp.w, &tmp.h);
    return tmp;
}


void BasicSdl1::Remove(DrawerSdl* child)
{
    for (auto it = m_Drawers.begin(); it != m_Drawers.end(); ++it)
    {
        if (*it == child)
        {
            m_Drawers.erase(it);
            break;
        }
    }
}
