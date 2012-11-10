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

#include "view/layer/virtualsurface.h"
#include "view/theme/animations.h"
#include "view/datatypes/rect.h"


using namespace std;


AnimationsList::AnimationsList() : List("AnimationsList")
{
};


AnimationsList::~AnimationsList()
{
    /*
    Animation* to_delete = GetFirstAnimation();
    do{
        delete(to_delete);
    }while((to_delete=GetNextAnimation()) != NULL);
    */
    reset();
}


void AnimationsList::reset()
{
//    for(Current=Begin; Begin!=End; Current++)
    {
        RemoveCurrentAnimation();
    }
}


void AnimationsList::AppendAnimation(Animation* a)
{
}


bool AnimationsList::Update(Vertex* layout)
{
    bool changed = false;
    Animation* animation = GetFirstAnimation();
    while((GetNextAnimation()) != NULL)
    {
        if(animation->execute(layout))
        {
            changed = true;
        }
    }

    return changed;
}


void AnimationsList::RemoveCurrentAnimation()
{
}


Animation* AnimationsList::GetFirstAnimation()
{
    return GetCurrentAnimation();
}


Animation* AnimationsList::GetNextAnimation()
{
    return GetCurrentAnimation();
}


Animation* AnimationsList::GetCurrentAnimation()
{
    return NULL;
}


//-----------------------------------------------
//-----------------------------------------------


VirtualSurface::VirtualSurface(string name) : Vertex(name)
{
    type(VIRTUALSURFACE);
    Changed = true;
    Buffer = NULL;
    Parent = NULL;
    SizeAndPositionOnBuffer.x = SizeAndPositionOnBuffer.y =
            SizeAndPositionOnBuffer.w = SizeAndPositionOnBuffer.h = 0;
}


VirtualSurface::~VirtualSurface()
{
    SDL_FreeSurface(Buffer);
}


void VirtualSurface::MapSurface(Rel_Rect* src_rect, SDL_Rect &tgt_rect, SDL_Surface*& tgt_surface)
{
    if(BufferType == COLLECTOR)
    {
        // Get the absolute position on the current buffer
        tgt_rect = SizeAndPositionOnBuffer;
        Multiply(src_rect, &tgt_rect);
        tgt_surface = GetBuffer();
        // "Updated" must be true in this case
        return;
    }

    // TODO: surface mapped incorrectly for List having BufferType OVERLAY

    // Get the next layers buffer and absolute position
    Rel_Rect* sap = GetRect(SIZEANDPOSITION, get(LAYOUT, ANY));
    Multiply(src_rect, sap);
    if(Parent)
        Parent->MapSurface(src_rect, tgt_rect, tgt_surface);
}


void VirtualSurface::Show(SDL_Rect* rect_abs_on_buffer, Rel_Rect* rect_relative_to_parent)
{
    if(Updated || !Parent)
        return;

    SDL_Rect tgt_rect;
    // Calculate position of the excerpt on the parent layer
    SDL_Surface* tgt_surface = NULL;
    Parent->MapSurface(rect_relative_to_parent, tgt_rect, tgt_surface);

    if(Parent->BufferType == OVERLAY)
        Parent->Show(&tgt_rect, rect_relative_to_parent);

    // DrawObject as parameter for positioning and alpha values
    BlitSurface(GetBuffer(), rect_abs_on_buffer, tgt_surface, &tgt_rect);
}


void VirtualSurface::BlitSurface
(
        SDL_Surface* source,
        SDL_Rect* src_pos,
        SDL_Surface* target,
        SDL_Rect* tgt_pos
)
{
    // Recalculate also the position
    SDL_SetClipRect(target, tgt_pos);
    if((source != NULL) && (target != NULL))
    {
//                SDL_SetAlpha(source, SDL_SRCALPHA, 128);
//                SDL_SetAlpha(target, SDL_SRCALPHA, 128);
        SDL_BlitSurface(source, src_pos, target, tgt_pos);
    }
}


void VirtualSurface::SetSize(SDL_Rect size)
{
    // Store only the size, position from layout
    Rel_Rect* sap = GetRect(SIZEANDPOSITION, get(LAYOUT, ANY));
    if(sap)
        Multiply(sap, &size);
    if((size.w!=SizeAndPositionOnBuffer.w) || (size.h!=SizeAndPositionOnBuffer.h))
    {
        SizeAndPositionOnBuffer.w = size.w;
        SizeAndPositionOnBuffer.h = size.h;
        SDL_FreeSurface(Buffer);
        Buffer = NULL;
        Changed = true;
    }
}


SDL_Rect VirtualSurface::GetSize()
{
    return SizeAndPositionOnBuffer;
}


void VirtualSurface::SetBufferType(buffer_type bt)
{
    if((BufferType != NONE) && (bt == NONE))
    {
        SDL_FreeSurface(Buffer);
    }
    BufferType = bt;
}


SDL_Surface*& VirtualSurface::GetBuffer()
{
    if(Buffer == NULL)
    {
        Buffer = SDL_GetVideoSurface();
        if(Parent)
        {
            const SDL_PixelFormat& fmt = *(Buffer->format);
            // Draw on buffer independent of the position
            Buffer = SDL_CreateRGBSurface(SDL_DOUBLEBUF|SDL_HWSURFACE,
                                          SizeAndPositionOnBuffer.w, SizeAndPositionOnBuffer.h,
                                          fmt.BitsPerPixel,
                                          fmt.Rmask, fmt.Gmask, fmt.Bmask, fmt.Amask);
        }
    }
    return Buffer;
}


void VirtualSurface::SetBuffer(SDL_Surface* buffer)
{
    SDL_FreeSurface(Buffer);
    Buffer = buffer;
}


void VirtualSurface::SetParent(VirtualSurface* parent)
{
    Parent = parent;
}
