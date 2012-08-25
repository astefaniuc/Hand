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


Link::Link(string name, string type, bool is_multi) : List(name)
{
    IsMulti = is_multi;
    SetType(type);
}


bool Link::Set(Vertex* val)
{
    if(!IsMulti && (size()>1))
        // TODO: delete or don't the linked element?
        pop_back();
    return Vertex::Set(val);
}


bool Link::Add(Vertex* val)
{
    if(IsMulti)
        return Add(val);
    if(size() > 1)
        return false;
    return Vertex::Add(val);
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


void Link::MakeMultiLink(bool cond)
{
    if(IsMulti == cond)
        return;
    if(IsMulti)
        while(size() > 2)
            // TODO: delete or don't the linked elements?
            pop_back();

    IsMulti = cond;
}


bool Link::IsMultiLink()
{
    return IsMulti;
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


Relation::Relation(string name) : List(name)
{
    SetType(RELATION);
}


bool Relation::Set(Vertex* item)
{
    Reset();
    push_back(item);
    return true;
}


bool Relation::IsOpen(Search* search)
{
    // Does it match in its role as relation
    RegularExpression* se = search->GetSearchRelation();
    if(se && !se->Matches(Name))
        // Don't look further if it's not the right relation
        return false;
    return true;
}
