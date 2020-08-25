#ifndef HAND_THEMES_BASIC_DRAWERS_H
#define HAND_THEMES_BASIC_DRAWERS_H

#include "virtualsurface.h"


class ButtonDrawer : public VirtualSurface
{
public:
    ButtonDrawer(Default* theme) : VirtualSurface(theme) {}

protected:
    void DrawSurface() override;
};


class ListDrawer : public VirtualSurface
{
public:
    ListDrawer(Default* theme) : VirtualSurface(theme) {}

protected:
    void DrawSurface() override;
};


class TextDrawer : public VirtualSurface
{
public:
    TextDrawer(Default* theme) : VirtualSurface(theme) {}

    void SetText(const std::string& item) { m_ViewItem = item; }

protected:
    void DrawSurface() override;

    std::string m_ViewItem;

private:
    const Rgb& GetFontColor();
};


class ViewDrawer : public VirtualSurface
{
public:
    ViewDrawer(Default* theme) : VirtualSurface(theme) {}

protected:
    void DrawSurface() override;
};

#endif // HAND_THEMES_BASIC_DRAWERS_H
