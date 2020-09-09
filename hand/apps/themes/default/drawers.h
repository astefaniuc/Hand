#ifndef HAND_THEMES_BASIC_DRAWERS_H
#define HAND_THEMES_BASIC_DRAWERS_H

#include "virtualsurface.h"


class Default;

class TextDrawer : public VirtualSurface
{
public:
    TextDrawer(Default* theme, Collection* config) : VirtualSurface(config), m_Theme(theme) {}

protected:
    void Draw(bool) override;

    SDL_Surface* RenderText(const std::string& text, int size, const Rgb& color) const;
    const Rgb& GetFontColor() const;
    Default* m_Theme;
};

#endif // HAND_THEMES_BASIC_DRAWERS_H
