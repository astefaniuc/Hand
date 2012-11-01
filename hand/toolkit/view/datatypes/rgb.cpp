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

#include "view/datatypes/rgb.h"
#include "graph/search/search.h"
#include "graph/data.h"


using namespace std;


Rgb::Rgb(string name, Uint8 r_, Uint8 g_, Uint8 b_) : Vertex(name)
{
    type(RGB);
    // For now add plain data directly
    add(new Data<Uint8>("r", r_));
    add(new Data<Uint8>("g", g_));
    add(new Data<Uint8>("b", b_));
    r = r_;
    g = g_;
    b = b_;
}


void Rgb::reset()
{
    r = ((Data<Uint8>*)get("r"))->get();
    g = ((Data<Uint8>*)get("g"))->get();
    b = ((Data<Uint8>*)get("b"))->get();
}


Rgb* GetRgb(string name, Vertex* tree)
{
    Vertex* rgb = tree->get(ANY, "Color");
    if(rgb)
        rgb = rgb->get();
    return dynamic_cast<Rgb*>(rgb);
}
