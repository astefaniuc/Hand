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

#ifndef HAND_RECT_H
#define HAND_RECT_H

#include "functoidnode.h"


using namespace std;


#define GUI_TYPE_RECT "GUI_TYPE_RECT"


class Rect : public FunctoidNode
{
    public:
        Rect(string name, double x_, double y_, double w_, double h_);
        Rect();
        ~Rect(){};

        void Init(double x_, double y_, double w_, double h_);
        void Reset();
        void Multiply(Rect* factor);
        void MultiplyTo(SDL_Rect& abs_rect);
        double x;
        double y;
        double w;
        double h;
};


// Helper func for extracting a Rect from a Functoid tree
Rect* GetRect(string rect_name, Functoid* from_tree);

#endif /* HAND_RECT_H */
