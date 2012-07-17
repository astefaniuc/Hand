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

#include "functoidlist.h"
#include "functoidsearch.h"


using namespace std;


FunctoidList::FunctoidList(string name) : Functoid(name)
{
    SetType(TYPE_FUNCTOIDLIST);
};


FunctoidList::~FunctoidList()
{
    CleanUp();
}


Functoid* FunctoidList::Find(string name, int max_depth)
{
    int depth = 0;
    Functoid* result = NULL;
    while(depth <= max_depth)
    {
        result = _Find(name, depth);

        if(result)
            break;
        depth++;
    }
    return result;
}


Functoid* FunctoidList::_Find(string name, int depth)
{
    if(depth < 0)
        return NULL;
    if(depth == 0)
    {
        if(Name == name)
            return this;
        return NULL;
    }

    Functoid* ret = NULL;
    uint s = size();
    --depth;
    for(uint i=0; i<s; i++)
    {
        ret = at(i)->_Find(name, depth);
        if(ret != NULL)
            break;
    }
    return ret;
}


bool FunctoidList::IsOpen(FunctoidSearch* search)
{
    // The search cookie should be the last element
    Functoid* last = back();
    if(last && (last->GetName()==search->GetCookieName()))
        // Already searched from different branch
        return false;
    return true;
}


bool FunctoidList::IsList()
{
    // Do nothing
    return true;
}


Functoid* FunctoidList::Find(SearchExpression* expression)
{
    // "Plain" find, don't descend
    uint s = size();
    Functoid* ret;
    for(uint i=0; i<s; i++)
    {
        ret = at(i);
        if(expression->Matches(ret->GetName()))
            return ret;
    }
    return NULL;
}


Functoid* FunctoidList::Get(string s)
{
    FunctoidIterator curr;
    FunctoidIterator _end = end();
    for(curr=begin(); curr!=_end; curr++)
    {
        if((*curr)->GetName() == s)
            return (*curr);
    }
    return NULL;
}


Functoid* FunctoidList::Get(uint i)
{
    --i;
    if(i < size())
        return at(i);
    return NULL;
}


bool FunctoidList::Set(Functoid* child)
{
    if(!child)
        return false;

    string s = child->GetName();
    FunctoidIterator curr = begin();
    while(curr!=end())
    {
        // TODO: is Type relevant?
        if((*curr)->GetName() == s)
        {
            delete(*curr);
            erase(curr);
        }
        else
            curr++;
    }
    push_back(child);
    return true;
}


bool FunctoidList::Add(Functoid* child)
{
    if(!child)
        return false;
    push_back(child);
    return true;
}


bool FunctoidList::Delete(Functoid* child)
{
    if(!Detach(child))
        return false;

    delete(child);
    return true;
}


bool FunctoidList::Attach(Functoid* child)
{
    Add(child);
    return true;
}


bool FunctoidList::Detach(Functoid* child)
{
    if(!child) return false;

    FunctoidIterator curr;
    for(curr=begin(); curr!=end(); curr++)
    {
        if((*curr) == child)
        {
            erase(curr);
            return true;
        }
    }
    return false;
}


string FunctoidList::GetAsString()
{
    string ret = "";
    uint s = size();
    for(uint i=0; i<s; i++)
        // Recursively print all childrens:
        ret += at(i)->GetAsString();
    return ret;
}


void FunctoidList::CleanUp()
{
    uint s = size();
    for(uint i=0; i<s; i++)
        // Recursivelly delete all childrens:
        delete at(i);
    if(s != 0)
        clear();
}
