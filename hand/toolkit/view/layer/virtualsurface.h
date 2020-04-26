#ifndef HAND_VIEW_LAYER_VIRTUALSURFACE_H
#define HAND_VIEW_LAYER_VIRTUALSURFACE_H

#include <SDL/SDL.h>
#include "graph/list.h"


class Animation;

class AnimationsList : public List
{
public:
    AnimationsList() : List("AnimationsList") {}
    ~AnimationsList();
    void reset();
    bool Update(Vertex* layout);
    void AppendAnimation(Animation* to_append);
    void RemoveCurrentAnimation();
    Animation* GetFirstAnimation();
    Animation* GetNextAnimation();
    Animation* GetCurrentAnimation();
};


enum buffer_type
{
    NONE,
    OVERLAY,
    COLLECTOR
};


class Rel_Rect;

class VirtualSurface : public Vertex
{
public:
    VirtualSurface(const std::string& name);
    virtual ~VirtualSurface();

    void Show(SDL_Rect* rect_abs_on_buffer, Rel_Rect* rect_relative_to_parent);

    // Set coordinates and size relative to the parent layer
    virtual void SetSize(SDL_Rect size);
    SDL_Rect GetSize() { return CoordinatesOnBuffer; }

    virtual void SetBufferType(buffer_type bt);
    SDL_Surface* GetBuffer();
    virtual void SetBuffer(SDL_Surface* buffer);

    void MapSurface(Rel_Rect* src_rect, SDL_Rect& tgt_rect, SDL_Surface*& tgt_surface);

    void SetParent(VirtualSurface* parent);

    void BlitSurface(
            SDL_Surface* source, SDL_Rect* source_pos,
            SDL_Surface* target, SDL_Rect* target_pos);

    AnimationsList Animations;
    bool Changed = true;

protected:
    buffer_type BufferType = NONE;
    SDL_Rect CoordinatesOnBuffer;
    VirtualSurface* Parent = nullptr;
    SDL_Surface* Buffer = nullptr;
    bool Updated = false;
};

#endif // HAND_VIEW_LAYER_VIRTUALSURFACE_H
