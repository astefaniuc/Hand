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

#ifndef HAND_RGB_H
#define HAND_RGB_H

#include "graph/list.h"
#include <SDL/SDL.h>


#define RGB "RGB"


class Rgb : public Vertex
{
    public:
        Rgb(std::string name, Uint8 r_, Uint8 g_, Uint8 b_);
        ~Rgb(){};
        void Reset();

        Uint8 r;
        Uint8 g;
        Uint8 b;
};


// Helper func for extracting a Rgb from a Vertex tree
Rgb* GetRgb(std::string rgb_name, Vertex* from_tree);


#endif /* HAND_RGB_H */
