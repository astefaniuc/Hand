#ifndef HAND_THEMES_DEFAULT_H
#define HAND_THEMES_DEFAULT_H

#include "view/theme.h"
#include "graph/collection.h"
#include <SDL/SDL_ttf.h>
#include <map>


#define FONT_FILE "/usr/share/fonts/truetype/freefont/FreeSans.ttf"


class Default : public Theme
{
public:
    Default();
    ~Default();

    HmiItem* GetHmi() override { return &m_Hmi; }

    virtual TTF_Font* GetFont(int size);
//    virtual void GetFontHeight(HmiItem* layout, unsigned& max_size);

    Drawer* GetButtonDrawer() override;
    Drawer* GetListDrawer() override;
    Drawer* GetTextDrawer() override;
    Drawer* GetViewDrawer() override;

protected:
    Collection m_Hmi;
    Collection m_Buttons;
    Collection m_Lists;
    Collection m_Texts;
    Collection m_Views;

private:
    // Stores once rendered fonts
    std::map<int, TTF_Font*> Fonts;
};

#endif // HAND_THEMES_DEFAULT_H
