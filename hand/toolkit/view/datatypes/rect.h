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

#ifndef VIEW_DATATYPES_RECT_H
#define VIEW_DATATYPES_RECT_H

#include <SDL/SDL.h>
#include "graph/data.h"


struct Rel_Rect
{
        Rel_Rect(double x_, double y_, double w_, double h_) : x(x_), y(y_), w(w_), h(h_){};
        Rel_Rect() : x(0.0), y(0.0), w(1.0), h(1.0){};
        Rel_Rect(Rel_Rect& c) : x(c.x), y(c.y), w(c.w), h(c.h){};

        double x;
        double y;
        double w;
        double h;
};


class Rect : public Data<Rel_Rect*>
{
    public:
        Rect(std::string name, Rel_Rect* val);
        ~Rect();

        using Data::set;
        bool set(Rel_Rect*);
        void reset();
};


class RectFactory : public Rect
{
    public:
        RectFactory(std::string name, double x_, double y_, double w_, double h_);
        ~RectFactory(){};
        using Data::get;
        Vertex* _get();
};


void Multiply(Rel_Rect* src, Rel_Rect* tgt);
void Multiply(Rel_Rect* src, SDL_Rect* tgt);
// Helper func for extracting a Rect from a Vertex tree
Rel_Rect* GetRect(std::string rect_name, Vertex* from_tree);

#endif /* VIEW_DATATYPES_RECT_H */
