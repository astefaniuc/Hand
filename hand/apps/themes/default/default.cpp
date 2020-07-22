#include "default.h"
#include "drawers.h"
#include "defines.h"
#include "virtualsurface.h"
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
    if (TTF_Init() == -1)
    {
        std::cout << TTF_GetError() << std::endl;
        exit(22);
    }

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
    std::map<int, TTF_Font*>::iterator curr = Fonts.begin();
    while(curr!=Fonts.end())
    {
        TTF_CloseFont((*curr).second);
        curr++;
    }
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
