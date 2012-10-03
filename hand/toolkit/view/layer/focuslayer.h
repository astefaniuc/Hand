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

#ifndef VIEW_LAYER_FOCUSLAYER_H
#define VIEW_LAYER_FOCUSLAYER_H

#include "view/layer/listlayer.h"


class Vertex;


class FocusLayer : public ListLayer
{
    public:
        FocusLayer();
        virtual ~FocusLayer();

        virtual void Draw(bool force);
        // Set pointer to a data tree node
//        void SetContent(Vertex* focus);

    protected:
        void Init();
        virtual void Configure(Vertex* parent_layout);

    private:
        VirtualSurface* GetControlSurface(uint index);
        Vertex* KeyList;
};

#endif /* VIEW_LAYER_FOCUSLAYER_H */
