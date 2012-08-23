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

#ifndef HAND_ANIMATIONS_H
#define HAND_ANIMATIONS_H

#include "base/handapp.h"


class Animation : public virtual HandApp
{
    public:
        Animation() : HandApp("Animation"){};
        virtual ~Animation(){};
//        void SizeAndPosition(SDL_Rect& current_size);
        // TODO: combine this with the Functoid-Execute()
        virtual bool Execute(Functoid* tgt) = 0;

    protected:
        virtual bool GetNextProgress() = 0;
        double Progress;
        double AnimationRate;
        int Repeat;
};


#endif /* HAND_ANIMATIONS_H */
