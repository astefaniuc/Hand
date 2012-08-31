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

    SetType(LIST);
}


bool List::Add(Vertex* child)
{
    return List::Get(PUBLICLIST)->Add(child);
}


bool List::Set(Vertex* child)
{
    return List::Get(PUBLICLIST)->Set(child);
}


bool List::Attach(Vertex* child)
{
    return List::Get(PUBLICLIST)->Attach(child);
}


Vertex* List::_Get()
{
    return List::Get(1);
}


Vertex* List::Get(string s)
{
    if(s == PUBLICLIST)
    {
        Vertex* child;
        uint i = 0;
        while((child=Vertex::Get(++i)) != NULL)
            if(child->Name() == s)
                return child;

        // Avoid endless recursion: Vertex + Attach
        child = new Vertex(s);
        Vertex::Attach(child);
        return child;
    }

    return List::Get(PUBLICLIST)->Get(s);
}


Vertex* List::Get(uint i)
{
    return List::Get(PUBLICLIST)->Get(i);
}


Vertex* List::Get(string type, string name)
{
    return List::Get(PUBLICLIST)->Get(type, name);
}


uint List::Size()
{
    return List::Get(PUBLICLIST)->Size();
}


void List::Reset()
{
    Vertex* pl = Vertex::Get(ANY, PUBLICLIST);
    if(pl)
    {
        Vertex::Detach(pl);
        delete(pl);
    }
}


bool List::Delete(Vertex* child)
{
    return List::Get(PUBLICLIST)->Delete(child);
}


bool List::Detach(Vertex* child)
{
    return List::Get(PUBLICLIST)->Detach(child);
}
