#ifndef HAND_THEMES_BASIC_DRAWERS_H
#define HAND_THEMES_BASIC_DRAWERS_H

#include "virtualsurface.h"


class TextDrawer : public DrawerSdl
{
public:
    TextDrawer(Default* theme, Collection* config) : DrawerSdl(theme, config) {}
    SDL_Rect CalculateSize(const SDL_Rect& offset) override;

    void Draw(bool) override;

protected:
    SDL_Surface* RenderText(const std::string& text, int size, const Rgb& color) const;
    const Rgb& GetFontColor() const;
    unsigned GetFontSize() const;
};

#endif // HAND_THEMES_BASIC_DRAWERS_H
