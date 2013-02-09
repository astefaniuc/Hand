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
        Target->Vertex::detach(this);
}


bool SearchCookie::attach(Vertex* target)
{
    if(target->size() == 0)
    {
        IsDeadBranch = true;
        return false;
    }
    Target = target;
    return Target->Vertex::attach(this);
}


Vertex* SearchCookie::get(uint i)
{
    SearchCookie* branch = (SearchCookie*)List::get(i);
    if(branch && branch->IsDeadBranch)
    {
        delete(branch);

        branch = (SearchCookie*)SearchCookie::get(i);
        if(!branch && (i==1))
            IsDeadBranch = true;
    }
    return branch;
}


Vertex* SearchCookie::_get()
{
    return Target;
}


bool SearchCookie::isOpen(Search* search)
{
    return false;
}
