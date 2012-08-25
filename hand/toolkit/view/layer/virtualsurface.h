/*
 *  Copyright 2012 Alex Stefaniuc
 *
 *  This file is part of Hand.
 *
 *  Hand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  Hand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with Hand. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HAND_VIRTUALSURFACE_H
#define HAND_VIRTUALSURFACE_H

#include "base/handapp.h"
#include <SDL/SDL.h>


class Animation;

class AnimationsList
{
    public:
        AnimationsList();
        ~AnimationsList();
        void Reset();
        bool Update(Vertex* layout);
        void AppendAnimation(Animation* to_append);
        void RemoveCurrentAnimation();
        Animation* GetFirstAnimation();
        Animation* GetNextAnimation();
        Animation* GetCurrentAnimation();

    private:
        std::vector<Animation*> Animations;
        std::vector<Animation*>::iterator Current;
        std::vector<Animation*>::iterator Begin;
        std::vector<Animation*>::iterator End;
};


// GUI types (Vertex tags)
#define VIRTUALSURFACE "VIRTUALSURFACE"

enum buffer_type
{
    NONE,
    OVERLAY,
    COLLECTOR
};


class Theme;
class Rect;


class VirtualSurface : public HandApp
{
    public:
        VirtualSurface(std::string name);
        ~VirtualSurface();
        virtual void Show(SDL_Rect* rect_abs_on_buffer,
                          Rect* rect_relative_to_parent);
        // Set coordinates and size relative to the parent layer
        virtual void SetSize(SDL_Rect size);
        virtual SDL_Rect _GetSize();
        virtual void SetBufferType(buffer_type bt);
        virtual SDL_Surface*& GetBuffer();
        virtual void SetBuffer(SDL_Surface* buffer);
        virtual void MapSurface(Rect* src_rect,
                                SDL_Rect& tgt_rect,
                                SDL_Surface*& tgt_surface);
        void SetParent(VirtualSurface* parent);
    protected:
        virtual void BlitSurface(SDL_Surface* source,
                                 SDL_Rect* source_pos,
                                 SDL_Surface* target,
                                 SDL_Rect* target_pos);

    public:
        AnimationsList Animations;
        bool Changed;
    protected:
        buffer_type BufferType;
        SDL_Rect SizeAndPositionOnBuffer;
        VirtualSurface* Parent;
        SDL_Surface* Buffer;
        bool Updated;
};


#endif /* HAND_VIRTUALSURFACE_H */
