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

#ifndef VIEW_SCREEN_H
#define VIEW_SCREEN_H

#include <SDL/SDL.h>
#include "base/handapp.h"


class Screen : public virtual HandApp
{
    public:
        Screen(void);

        // App mode interface
        bool ToggleFullscreen(Vertex* ignore);
        bool SetFullscreen(void);
        bool SetWindowed(void);
        void Init(void);
        // GetResolution()
        // GetSize()/ GetPosition()
        SDL_Rect GetResolution(void);
        // GetBuffer()
        // Re-calculate position for all layer manager
        bool SetLayerManagerPositions();
        SDL_Surface* GetSurface(void);
        bool ShowSurface(void);

    private:
        void InitSDL(void);
        // The whole screen
        SDL_Surface* Surface;
        // TODO: we need only one "Size" member and the "IsFullscreen" to store
        // all sizes we need ("Surface" stores the second size)
        bool IsFullscreen;
};

#endif /* VIEW_SCREEN_H */
