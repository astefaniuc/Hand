#ifndef HAND_VIEW_THEME_THEME_H
#define HAND_VIEW_THEME_THEME_H

#include "SDL/SDL_ttf.h"
// #include "SDL/SDL_Image.h"
#include <map>
#include "base/module.h"
#include "graph/collection.h"
#include "view/drawer.h"


class Rgb;
class Rel_Rect;
class VirtualSurface;

class Theme : public Module
{
public:
    Theme();
    ~Theme();

    bool Draw(Layout* a_input);

protected:
    virtual TTF_Font* GetFont(int size);
//    virtual void GetFontHeight(HmiItem* layout, unsigned& max_size);

    // Drawing interface
    virtual SDL_Surface* RenderText(const std::string& text, int size, Rgb* color);

    // Changes x and y of source
    void PlaceCentered(SDL_Surface* source, SDL_Rect& target, Rel_Rect& out);

    // Stores once rendered fonts
    std::map<int, TTF_Font*> Fonts;
};

#endif // HAND_VIEW_THEME_THEME_H
