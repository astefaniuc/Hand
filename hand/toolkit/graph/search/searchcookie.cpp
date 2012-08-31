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


extern Pool* CookiePool;


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


bool SearchCookie::Attach(Vertex* target)
{
    if(target->GetSize() == 0)
    {
        IsDeadBranch = true;
        return false;
    }
    Target = target;
    return Target->Vertex::Attach(this);
}


Vertex* SearchCookie::Get(uint i)
{
    SearchCookie* branch = (SearchCookie*)List::Get(i);
    if(branch && branch->IsDeadBranch)
    {
        Detach(branch);
        CookiePool->Take(branch);

        branch = (SearchCookie*)SearchCookie::Get(i);
        if(!branch && (i==1))
            IsDeadBranch = true;
    }
    return branch;
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

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


Vertex* Pool::Get()
{
    Vertex* cookie = List::Get(1);
    if(cookie)
    {
        Detach(cookie);
        cookie->Reset();
        return cookie;
    }
    return new SearchCookie();
}


void Pool::Take(Vertex* cookie)
{
    if(GetSize() >= MAX_POOL_SIZE)
    {
        delete(cookie);
        return;
    }

    // Add to the pool
    Attach(cookie);

    Vertex* child;
    while((child=cookie->Get(1)) != NULL)
    {
        cookie->Detach(child);
        Take(child);
    }
}


void Pool::Reset()
{
    Vertex* child;
    uint i = 0;
    while((child=List::Get(++i)) != NULL)
        child->Reset();
}

