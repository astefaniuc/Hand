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

#include "graph/list.h"


using namespace std;


List::List(string name) : Vertex(name)
{
    /*
       Delayed initialization of the public list,
       allows initialization with a customized list
    */

    type(LIST);
}


bool List::add(Vertex* child)
{
    return List::get(PUBLIC)->add(child);
}


bool List::set(Vertex* child)
{
    return List::get(PUBLIC)->set(child);
}


bool List::attach(Vertex* child)
{
    return List::get(PUBLIC)->attach(child);
}


Vertex* List::_get()
{
    return List::get(1);
}


Vertex* List::get(string s)
{
    if(s == PUBLIC)
    {
        Vertex* child;
        uint i = 0;
        while((child=Vertex::get(++i)) != NULL)
            if(child->name() == s)
                return child;

        // Avoid endless recursion: Vertex + Attach
        child = new Vertex(s);
        Vertex::attach(child);
        return child;
    }

    return List::get(PUBLIC)->get(s);
}


Vertex* List::get(uint i)
{
    return List::get(PUBLIC)->get(i);
}


Vertex* List::get(string type, string name)
{
    return List::get(PUBLIC)->get(type, name);
}


uint List::size()
{
    return List::get(PUBLIC)->size();
}


void List::reset()
{
    Vertex* pl = Vertex::get(ANY, PUBLIC);
    if(pl)
        pl->Vertex::reset();
}


bool List::remove(Vertex* child)
{
    return List::get(PUBLIC)->remove(child);
}


bool List::detach(Vertex* child)
{
    return List::get(PUBLIC)->detach(child);
}
