#ifndef HAND_VIEW_DRAWER_H
#define HAND_VIEW_DRAWER_H
// TODO: remove dependency on SDL here
#include <SDL2/SDL.h>


class Layer;
class Theme;

class Drawer
{
public:
    virtual void Exit(Layer*) = 0;

    virtual void Draw(SDL_Surface* buffer) = 0;
    virtual SDL_Rect ComputeSize(const SDL_Rect& content) = 0;
    virtual SDL_Rect GetContentSize(const SDL_Rect& outer) = 0;

    virtual void SetLayer(Layer* layer) { m_Layer = layer; }

protected:
    virtual ~Drawer() = default;

    Layer* m_Layer = nullptr;
};

#endif // HAND_VIEW_DRAWER_H
