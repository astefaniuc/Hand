#ifndef HAND_THEMES_DEFAULT_H
#define HAND_THEMES_DEFAULT_H

#include "include/theme.h"


class Default : public Theme
{
public:
    Default();

protected:
    // Layout creators
    bool GetViewLayout(Vertex* out);
    bool GetFramedListLayout(Vertex* out);
    bool GetListLayout(Vertex* out);
    bool GetButtonLayout(Vertex* out);
    bool GetContainerLayout(Vertex* out);
    bool GetFrameLayout(Vertex* out);
    bool GetBackgroundLayout(Vertex* out);
    bool GetTextLayout(Vertex* out);

    // Drawing methods
    bool ColorSurface(Vertex* drawing);
    bool DrawFrame(Vertex* drawing);
    bool DrawList(Vertex* drawing);
    bool DrawText(Vertex* drawing);

    bool DrawView(Vertex* drawing);

    std::string GetString(Vertex* vs);
};

/*
class ZoomIn : public virtual Animation
{
    public:
        ZoomIn();
        ~ZoomIn(){};
        virtual bool GetSize(SDL_Rect& size);
        virtual bool GetNextProgress();
};
*/

#endif // HAND_THEMES_DEFAULT_H
