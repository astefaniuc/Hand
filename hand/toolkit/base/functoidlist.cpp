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
    // reserve the first list entry for runtime information
    Add(new Functoid("Runtime"));
    SetType(TYPE_FUNCTOIDLIST);
}


FunctoidList::~FunctoidList()
{
    CleanUp();
}


void FunctoidList::SetType(string type)
{
    if(type != "")
        Get(0)->SetType(type);
}


bool FunctoidList::Add(string relation_name, Functoid* child)
{
    Relation* r = dynamic_cast<Relation*>(Get(relation_name));
    if(!r)
    {
        // Create relation (link functoid) if not existing
        r = new Relation(relation_name);
        Add(r);
    }
    // Add the parent to the current functoid
    return r->Add(child);
}


bool FunctoidList::Set(string relation_name, Functoid* child)
{
    Relation* r = dynamic_cast<Relation*>(Get(relation_name));
    if(!r)
    {
        // Create relation (link functoid) if not existing
        r = new Relation(relation_name);
        // TODO: alternatively call Set() to have a unique child of this name
        Add(r);
    }
    return r->Set(child);
}


Functoid* FunctoidList::Get(string s)
{
    Functoid* ret = Functoid::Get(s);
    if(!ret)
        // Search hidden elements
        return Get(0)->Get(s);
    return ret;
}


Functoid* FunctoidList::Get(string relation, string element)
{
    FunctoidIterator curr;
    FunctoidIterator _end = end();
    for(curr=begin(); curr!=_end; curr++)
        if(((*curr)->GetName()==relation) && (*curr)->IsType(FUNCTOIDRELATION))
            return (*curr)->Get(element);

    return NULL;
}


Functoid* FunctoidList::Get(uint i)
{
    if(i < size())
        return at(i);
    return NULL;
}


bool FunctoidList::IsOwner(Functoid* caller)
{
    return Get(0)->IsOwner(caller);
}


void FunctoidList::CleanUp()
{
    Functoid* curr;
    // Don't delete the parent(s)
    Functoid* parent = Get(TAG_RELATION_PARENT);
    if(parent)
        parent->clear();

    uint s = size();
    for(uint i=0; i<s; i++)
    {
        curr = at(i);
        if(curr->IsOwner(this))
            // Recursively delete all childrens:
            delete curr;
    }
    // Avoid that FunctoidList destructor deletes the parent(s)
    if(s != 0)
        clear();
}


bool FunctoidList::Detach(Functoid* child)
{
    Relation* relation;
    FunctoidIterator curr;
    for(curr=begin(); curr!=end(); curr++)
    {
        if((*curr) == child)
        {
            erase(curr);
            return true;
        }
        relation = dynamic_cast<Relation*>(*curr);
        if(relation && relation->Detach(child))
            return true;
    }
    return false;
}



Factory* FunctoidList::GetFactory()
{
    FunctoidSearch search;
    search.MaxDepth = 2;
    search.SetSearchRelation(RELATION_PRODUCER);
    search.SetSearchType(TYPE_FACTORY);
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
    SetType(FUNCTOIDRELATION);
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
