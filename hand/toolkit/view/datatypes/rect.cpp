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


using namespace std;


Rect::Rect(string name, Rel_Rect* val) : Data<Rel_Rect*>(name, val)
{
    type(RECT);
    // For now add plain data directly
    set(new Data<double>("x", Value->x));
    set(new Data<double>("y", Value->y));
    set(new Data<double>("w", Value->w));
    set(new Data<double>("h", Value->h));
}


Rect::~Rect()
{
    delete(Value);
}


bool Rect::set(Rel_Rect* val)
{
    Data<Rel_Rect*>::set(val);
    // For now add plain data directly
    ((Data<double>*)get("x"))->set(Value->x);
    ((Data<double>*)get("y"))->set(Value->y);
    ((Data<double>*)get("w"))->set(Value->w);
    ((Data<double>*)get("h"))->set(Value->h);
    return true;
}


void Rect::Reset()
{
    if(!Value)
        Value = new Rel_Rect();
    Value->x = ((Data<double>*)get("x"))->get();
    Value->y = ((Data<double>*)get("y"))->get();
    Value->w = ((Data<double>*)get("w"))->get();
    Value->h = ((Data<double>*)get("h"))->get();
}


void Multiply(Rel_Rect* src, Rel_Rect* tgt)
{
    if(!src || !tgt)
        return;
    tgt->x += (src->x * tgt->w);
    tgt->y += (src->y * tgt->h);
    tgt->w *= src->w;
    tgt->h *= src->h;
}


void Multiply(Rel_Rect* src, SDL_Rect* tgt)
{
    if(!src || !tgt)
        return;
    tgt->x += (src->x * tgt->w);
    tgt->y += (src->y * tgt->h);
    tgt->w *= src->w;
    tgt->h *= src->h;
}


Rel_Rect* GetRect(string name, Vertex* tree)
{
    Vertex* named = tree->get(ANY, name);
    if(named)
    {
        Rect* container = dynamic_cast<Rect*>(named->get());
        if(container)
            return container->get();
    }
    return NULL;
}
