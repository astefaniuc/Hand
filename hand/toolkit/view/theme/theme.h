#ifndef HAND_VIEW_THEME_THEME_H
#define HAND_VIEW_THEME_THEME_H

#include "SDL/SDL_ttf.h"
// #include "SDL/SDL_Image.h"
#include <map>
#include "base/handapp.h"


class Layout;
class Rgb;
class Rel_Rect;
class VirtualSurface;

class Theme : public HandApp
{
public:
    Theme(const std::string& name);
    virtual ~Theme();

    bool execute(Vertex* request) override;

protected:
    bool FillOut(Vertex* layout);

    // Helper methods
    virtual TTF_Font* GetFont(int size);
    virtual void GetFontHeight(Vertex* layout, uint& max_size);
    VirtualSurface* GetSurface(Vertex* layout);

    // Drawing interface
    virtual void FillRect(SDL_Surface* sf, SDL_Rect* r, Rgb* c);
    virtual SDL_Surface* RenderText(const std::string& text, int size, Rgb* color);

    // Changes x and y of source
    void PlaceCentered(SDL_Surface* source, SDL_Rect& target, Rel_Rect& out);

    // Stores once rendered fonts
    std::map<int, TTF_Font*> Fonts;
};

#endif // HAND_VIEW_THEME_THEME_H
