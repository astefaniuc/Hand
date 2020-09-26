#ifndef HAND_VIEW_DRAWER_H
#define HAND_VIEW_DRAWER_H
// TODO: remove dependency on SDL here
#include <SDL/SDL.h>


class Layer;
class Theme;

class Drawer
{
public:
    virtual ~Drawer() {}

    virtual void Draw(bool forced) = 0;
    virtual void DrawChild(Layer* child, bool forced) = 0;
    virtual SDL_Rect CalculateSize(SDL_Rect& content, SDL_Rect& total) = 0;
    virtual SDL_Rect GetFrameOffset() = 0;
    virtual void SetLayer(Layer* layer) { m_Layer = layer; }

protected:
    Layer* m_Layer = nullptr;
};

#endif // HAND_VIEW_DRAWER_H
