#ifndef HAND_VIEW_THEME_H
#define HAND_VIEW_THEME_H

#include "base/module.h"
#include "view/drawer.h"


class Theme : public Module
{
public:
    virtual void InitScreen(Layer* root) = 0;
    virtual void UpdateScreen() = 0;

    virtual Drawer* GetButtonDrawer(Layer*) = 0;
    virtual Drawer* GetDataDrawer(Layer*) = 0;
    virtual Drawer* GetChordDrawer(Layer*) = 0;
    virtual Drawer* GetListDrawer(Layer*) = 0;
    virtual Drawer* GetTextDrawer(Layer*) = 0;
    virtual Drawer* GetViewDrawer(Layer*) = 0;
};

#endif // HAND_VIEW_THEME_H
