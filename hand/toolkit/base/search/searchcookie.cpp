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

#include "searchcookie.h"
#include "functoidsearch.h"


using namespace std;


SearchCookie::SearchCookie() : FunctoidNode(SEARCHCOOKIE)
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


bool SearchCookie::IsOpen(FunctoidSearch* search)
{
    return false;
}


bool SearchCookie::Add(Functoid* child)
{
    push_back(child);
    ((SearchCookie*)child)->Parent = this;
    return true;
}


bool SearchCookie::Detach(Functoid* ignore)
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


Functoid* Pool::Get()
{
    if(size()>1)
    {
        Functoid* cookie = back();
        pop_back();
        return cookie;
    }
    return new SearchCookie();
}


void Pool::Take(Functoid* cookie)
{
    // Detach the Target functoid
    cookie->Detach(NULL);

    ((SearchCookie*)cookie)->Parent = NULL;
    // Add to the pool
    push_back(cookie);
    ((SearchCookie*)cookie)->IsDeadBranch = false;
    FunctoidList* cookie_l = (FunctoidList*)cookie;
    uint i = cookie_l->size();
    while(i > 1)
    {
        Take(cookie_l->Get(--i));
        cookie_l->pop_back();
    }
}
