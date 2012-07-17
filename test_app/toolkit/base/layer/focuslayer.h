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

#ifndef HAND_FOCUSLAYER_H
#define HAND_FOCUSLAYER_H

#include "listlayer.h"


using namespace std;


class Functoid;


class FocusLayer : public ListLayer
{
    public:
        // Init with link to the parent layer
        FocusLayer();
        virtual ~FocusLayer();

        virtual void Draw(bool force);
        // Set pointer to a data tree node
//        void SetContent(Functoid* focus);

    protected:
        void Init();
        virtual void Configure(Functoid* parent_layout);

    private:
        VirtualSurface* GetControlSurface(uint index);
        Functoid* KeyList;
};

#endif /* HAND_FOCUSLAYER_H */
