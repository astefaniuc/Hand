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

#include "graph/link.h"
#include "graph/search/search.h"


using namespace std;


Link::Link(string name, string type) : List(name)
{
    Type(type);
}


bool Link::Execute(Vertex* vs)
{
    uint i = 0;
    Vertex* child;
    while((child=Get(++i)) != NULL)
        // TODO: needs concrete use cases for MultiLink
        return child->Execute(vs);
    return false;
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


Relation::Relation(string name) : List(name)
{
    Type(RELATION);
}


bool Relation::Set(Vertex* target)
{
    List::Reset();
    return List::Set(target);
    return true;
}


bool Relation::IsOpen(Search* search)
{
    if(!Vertex::IsOpen(search))
        return false;
    // Does it match in its role as relation
    RegularExpression* se = search->GetSearchRelation();
    if(se && !se->Matches(Name()))
        // Don't look further if it's not the right relation
        return false;
    return true;
}
