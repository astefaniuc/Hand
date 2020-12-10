#ifndef HAND_THEMES_BASICSDL1_TEXT_H
#define HAND_THEMES_BASICSDL1_TEXT_H

#include "drawersdl.h"


class Text : public DrawerSdl
{
public:
    Text(BasicSdl1* theme, Hmi::List* config) : DrawerSdl(theme, config) {}
    ~Text() { SDL_FreeSurface(m_Buffer); }

    SDL_Rect ComputeSize(const SDL_Rect& content) override;

    void Draw(SDL_Surface* buffer) override;

protected:
    SDL_Surface* RenderText(const std::string& text, int size, const Rgb& color) const;
    const Rgb& GetFontColor() const;
    unsigned GetFontSize() const;
};

#endif // HAND_THEMES_BASICSDL1_TEXT_H
