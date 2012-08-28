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
#include "graph/search/search.h"


using namespace std;


SearchCookie::SearchCookie() : List(SEARCHCOOKIE)
{
    Target = NULL;
    IsDeadBranch = false;
}


SearchCookie::~SearchCookie()
{
    if(Target)
        Target->Vertex::Detach(this);
}


bool SearchCookie::IsOpen(Search* search)
{
    return false;
}


void SearchCookie::Reset()
{
    if(Target)
        Target->Vertex::Detach(this);
    Target = NULL;
    IsDeadBranch = false;
}


bool SearchCookie::MarkDeathBranch()
{
    SearchCookie* c;
    uint i = 0;
    while((c=(SearchCookie*)Get(++i)) != NULL)
        if(!c->IsDeadBranch)
            return false;

    IsDeadBranch = true;

    Vertex* p_rel = Vertex::Get(ANY, OWNER);
    if(p_rel)
    {
        SearchCookie* parent = dynamic_cast<SearchCookie*>(p_rel->Get(1));
        if(parent)
            return parent->MarkDeathBranch();
    }
    return false;
}

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


Vertex* Pool::Get()
{
    Vertex* ret = List::Get(1);
    if(ret)
    {
        Detach(ret);
        return ret;
    }
    return new SearchCookie();
}


void Pool::Take(Vertex* cookie)
{
    cookie->Reset();
    // Add to the pool
    Attach(cookie);

    Vertex* child;
    uint i = 0;
    while((child=cookie->Get(++i)) != NULL)
    {
        if(dynamic_cast<SearchCookie*>(child))
        {
            cookie->Detach(child);
            Take(child);
        }
    }
}
