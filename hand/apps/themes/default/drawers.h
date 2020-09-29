#ifndef HAND_THEMES_BASIC_DRAWERS_H
#define HAND_THEMES_BASIC_DRAWERS_H

#include "virtualsurface.h"


class TextDrawer : public DrawerSdl
{
public:
    TextDrawer(Default* theme, Collection* config) : DrawerSdl(theme, config) {}
    ~TextDrawer() { SDL_FreeSurface(m_Buffer); }

    SDL_Rect CalculateSize(SDL_Rect& content, SDL_Rect& total) override;

    void Draw(SDL_Surface* buffer, bool) override;

protected:
    SDL_Surface* RenderText(const std::string& text, int size, const Rgb& color) const;
    const Rgb& GetFontColor() const;
    unsigned GetFontSize() const;
};

#endif // HAND_THEMES_BASIC_DRAWERS_H
