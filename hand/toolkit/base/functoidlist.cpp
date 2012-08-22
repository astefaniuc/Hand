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
#include "factory.h"


using namespace std;


FunctoidList::FunctoidList(string name) : Functoid(name)
{
    // Reserve the first list entry for runtime information
    Attach(new Functoid("Runtime"));
    SetType(FUNCTOIDLIST);
}


FunctoidList::~FunctoidList()
{
    // Don't delete the parent(s); same source code as in ~Functoid()
    // because that destructor doesn't calls the overloaded method
    Functoid* p = Get(RELATION, OWNER);
    // CleanUp() doesn't work here
    if(p && (p->size()== 2))
        p->pop_back();
}


Functoid* FunctoidList::Get(string s)
{
    // Search public elements
    Functoid* ret = Functoid::Get(ANY, s);
    if(ret)
        return ret;
    // Search hidden elements
    ret = Get(RUNTIME)->Get(ANY, s);
    if(ret)
        return ret;
    // Return a new public relation
    ret = new Relation(s);
    Add(ret);
    return ret;
}


Functoid* FunctoidList::Get(uint i)
{
    if(i < size())
        return at(i);
    return NULL;
}


Functoid* FunctoidList::Get(string type, string name)
{
    Functoid* ret = Functoid::Get(type, name);
    if(!ret)
        return Get(RUNTIME)->Functoid::Get(type, name);
    return ret;
}


void FunctoidList::SetType(string type)
{
    if(type != "")
        Get(RUNTIME)->SetType(type);
}


string FunctoidList::GetType()
{
    return Get(RUNTIME)->GetType();
}


bool FunctoidList::IsType(string type)
{
    return Get(RUNTIME)->IsType(type);
}


bool FunctoidList::IsType(SearchExpression* se)
{
    return Get(RUNTIME)->IsType(se);
}


void FunctoidList::SetOwner(Functoid* owner)
{
    Get(RUNTIME)->SetOwner(owner);
}


bool FunctoidList::HasOwner(Functoid* caller)
{
    return Get(RUNTIME)->HasOwner(caller);
}


void FunctoidList::CleanUp()
{
    FunctoidIterator curr = begin();
    // Don't delete the runtime info
    curr++;
    while(curr!=end())
    {
        if((*curr)->HasOwner(this))
            delete((*curr));
        erase(curr);
    }
}


Factory* FunctoidList::GetFactory()
{
    FunctoidSearch search;
    search.MaxDepth = 2;
    search.SetSearchRelation(RELATION_PRODUCER);
    search.SetSearchType(FACTORY);
    if(search.Search(this))
        return dynamic_cast<Factory*>(search.GetFindings());
    return NULL;
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


Link::Link(string name, string type, bool is_multi) : FunctoidList(name)
{
    IsMulti = is_multi;
    SetType(type);
}


bool Link::Set(Functoid* val)
{
    if(!IsMulti && (size()>1))
        // TODO: delete or don't the linked element?
        pop_back();
    return Functoid::Set(val);
}


bool Link::Add(Functoid* val)
{
    if(IsMulti)
        return Add(val);
    if(size() > 1)
        return false;
    return Functoid::Add(val);
}


bool Link::Execute(Functoid* vs)
{
    uint i = 0;
    Functoid* child;
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


Relation::Relation(string name) : FunctoidList(name)
{
    SetType(RELATION);
}


bool Relation::Set(Functoid* item)
{
    CleanUp();
    push_back(item);
    return true;
}


bool Relation::IsOpen(FunctoidSearch* search)
{
    // Does it match in its role as relation
    SearchExpression* se = search->GetSearchRelation();
    if(se && !se->Matches(Name))
        // Don't look further if it's not the right relation
        return false;
    return true;
}
