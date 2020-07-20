#ifndef HAND_VIEW_THEME_H
#define HAND_VIEW_THEME_H

// #include "SDL/SDL_Image.h"
#include "base/module.h"
#include "graph/collection.h"
#include "view/drawer.h"




class Rgb;
class Rel_Rect;

class Theme : public Module
{
public:
    virtual Drawer* GetButtonDrawer() = 0;
    virtual Drawer* GetListDrawer() = 0;
    virtual Drawer* GetTextDrawer() = 0;
    virtual Drawer* GetViewDrawer() = 0;
};

#endif // HAND_VIEW_THEME_H
