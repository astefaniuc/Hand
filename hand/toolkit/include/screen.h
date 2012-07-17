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

#ifndef HAND_DRAWER_H
#define HAND_DRAWER_H

#include "handapp.h"


using namespace std;


class Screen : public virtual HandApp
{
    public:
        Screen(void);

        // App mode interface
        bool ToggleFullscreen(Functoid* ignore);
        bool SetFullscreen(void);
        bool SetWindowed(void);
        void Init(void);
        // GetResolution()
        void SetSize(Size*);
        // GetSize()/ GetPosition()
        SDL_Rect GetResolution(void);
        // GetBuffer()
        SDL_Surface* GetSurface(void);
        void ShowSurface(void);

    private:
        void InitSDL(void);
        // The whole screen
        SDL_Surface* Surface;
        // TODO: we need only one "Size" member and the "IsFullscreen" to store
        // all sizes we need ("Surface" stores the second size)
        Size WndSize;
        Size FullSize;
        bool IsFullscreen;
};

#endif /* HAND_DRAWER_H */
