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

    virtual void Draw(SDL_Surface* buffer) = 0;
    virtual void DrawChild(Layer* child) = 0;
    virtual SDL_Rect CalculateSize(SDL_Rect& content) = 0;
    virtual SDL_Rect GetContentSize(const SDL_Rect& outer) = 0;
    virtual void SetLayer(Layer* layer) { m_Layer = layer; }

protected:
    Layer* m_Layer = nullptr;
};

#endif // HAND_VIEW_DRAWER_H
