#ifndef HAND_VIEW_THEME_ANIMATIONS_H
#define HAND_VIEW_THEME_ANIMATIONS_H

#include "base/handapp.h"


class Animation : public virtual HandApp
{
public:
    Animation() : HandApp("Animation") {}

//    void Coordinates(SDL_Rect& current_size);
    virtual bool execute(Vertex* tgt) = 0;

protected:
    virtual bool GetNextProgress() = 0;

    double Progress = 0.0;
    double AnimationRate = 0.04;
    int Repeat = 0;
};


#endif // HAND_VIEW_THEME_ANIMATIONS_H
