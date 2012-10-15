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
    Type(RECT);
    Init(x_, y_, w_, h_);
}


Rect::Rect() : Vertex("No name")
{
    Type(RECT);
    Init(0.0, 0.0, 1.0, 1.0);
}


void Rect::Init(double x_, double y_, double w_, double h_)
{
    // For now add plain data directly
    Set(new Data<double>("x", x_));
    Set(new Data<double>("y", y_));
    Set(new Data<double>("w", w_));
    Set(new Data<double>("h", h_));
    x = x_;
    y = y_;
    w = w_;
    h = h_;
}


void Rect::Reset()
{
    x = ((Data<double>*)Get("x"))->Get();
    y = ((Data<double>*)Get("y"))->Get();
    w = ((Data<double>*)Get("w"))->Get();
    h = ((Data<double>*)Get("h"))->Get();
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
    Rect* ret = dynamic_cast<Rect*>(tree->Get(RECT, name));
    if(!ret)
    {
        // Add a default Rect
        ret = new Rect(SIZEANDPOSITION, 0, 0, 1, 1);
        tree->Set(ret);
    }

    return ret;
}
