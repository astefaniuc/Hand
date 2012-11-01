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

#include "view/datatypes/rect.h"
#include "graph/search/search.h"
#include "graph/data.h"


using namespace std;


Rect::Rect(string name, double x_, double y_, double w_, double h_) : Vertex(name)
{
    type(RECT);
    Init(x_, y_, w_, h_);
}


Rect::Rect() : Vertex("No name")
{
    type(RECT);
    Init(0.0, 0.0, 1.0, 1.0);
}


void Rect::Init(double x_, double y_, double w_, double h_)
{
    // For now add plain data directly
    set(new Data<double>("x", x_));
    set(new Data<double>("y", y_));
    set(new Data<double>("w", w_));
    set(new Data<double>("h", h_));
    x = x_;
    y = y_;
    w = w_;
    h = h_;
}


void Rect::reset()
{
    x = ((Data<double>*)get("x"))->get();
    y = ((Data<double>*)get("y"))->get();
    w = ((Data<double>*)get("w"))->get();
    h = ((Data<double>*)get("h"))->get();
}


void Rect::Multiply(Rect* factor)
{
    x += (factor->x * w);
    y += (factor->y * h);
    w *= factor->w;
    h *= factor->h;
}


void Rect::MultiplyTo(SDL_Rect& abs_rect)
{
    abs_rect.x += (abs_rect.w * x);
    abs_rect.y += (abs_rect.h * y);
    abs_rect.w *= w;
    abs_rect.h *= h;
}


Rect* GetRect(string name, Vertex* tree)
{
    Rect* ret = dynamic_cast<Rect*>(tree->get(RECT, name));
    if(!ret)
    {
        // Add a default Rect
        ret = new Rect(SIZEANDPOSITION, 0, 0, 1, 1);
        tree->set(ret);
    }

    return ret;
}
