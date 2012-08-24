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

#include "graph/search/searchcookie.h"
#include "graph/search/functoidsearch.h"


using namespace std;


SearchCookie::SearchCookie() : List(SEARCHCOOKIE)
{
    Target = NULL;
    Parent = NULL;
    IsDeadBranch = false;
}


SearchCookie::~SearchCookie()
{
    if(Target)
        Target->Detach(this);
}


bool SearchCookie::IsOpen(VertexSearch* search)
{
    return false;
}


bool SearchCookie::Add(Vertex* child)
{
    push_back(child);
    ((SearchCookie*)child)->Parent = this;
    return true;
}


bool SearchCookie::Detach(Vertex* ignore)
{
    bool ret = false;
    if(Target)
    {
        ret = Target->Detach(this);
        Target = NULL;
    }
    return ret;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


Vertex* Pool::Get()
{
    if(size()>1)
    {
        Vertex* cookie = back();
        pop_back();
        return cookie;
    }
    return new SearchCookie();
}


void Pool::Take(Vertex* cookie)
{
    SearchCookie* sc = dynamic_cast<SearchCookie*>(cookie);
    if(!sc)
        return;
    // Detach the Target functoid
    sc->Detach(NULL);

    sc->Parent = NULL;
    // Add to the pool
    push_back(sc);
    sc->IsDeadBranch = false;
    uint i = sc->size();
    while(i > 1)
    {
        Take(sc->Get(--i));
        sc->pop_back();
    }
}
