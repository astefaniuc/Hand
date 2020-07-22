#ifndef HAND_VIEW_THEME_H
#define HAND_VIEW_THEME_H

#include "base/module.h"
#include "view/drawer.h"


class Theme : public Module
{
public:
    virtual Drawer* GetButtonDrawer() = 0;
    virtual Drawer* GetListDrawer() = 0;
    virtual Drawer* GetTextDrawer() = 0;
    virtual Drawer* GetViewDrawer() = 0;
};

#endif // HAND_VIEW_THEME_H
