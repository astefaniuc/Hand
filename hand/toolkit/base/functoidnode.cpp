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

#include "functoidnode.h"
#include "functoidsearch.h"
#include "factory.h"


using namespace std;


FunctoidNode::FunctoidNode(string name) : FunctoidList(name)
{
    SetType(TYPE_FUNCTOIDNODE);
}


FunctoidNode::~FunctoidNode()
{
    CleanUp();
}


void FunctoidNode::SetType(string type)
{
    // Type as "relation"
    Add(TAG_TYPE, new FunctoidType(Type));
    // Store the latest type as string (returned by GetType())
    Functoid::SetType(type);
}


bool FunctoidNode::IsType(string type)
{
    if(Type==type)
        return true;
    Functoid* types = Get(TAG_TYPE);
    if(types && types->Get(type))
        return true;
    return false;
}


bool FunctoidNode::IsType(SearchExpression* type)
{
    if(!type)
        return false;
    if(Functoid::IsType(type))
        return true;
    Relation* type_node = dynamic_cast<Relation*>(Get(TAG_TYPE));
    if(!type_node)
        return false;
    return type_node->Find(type);
}


bool FunctoidNode::Add(string relation_name, Functoid* child)
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


bool FunctoidNode::Set(string relation_name, Functoid* child)
{
    Relation* r = dynamic_cast<Relation*>(Get(relation_name));
    if(r)
        r->clear();
    else
    {
        // Create relation (link functoid) if not existing
        r = new Relation(relation_name);
        // TODO: alternatively call Set() to have a unique child of this name
        Add(r);
    }
    return r->Set(child);
}


Functoid* FunctoidNode::Get(string relation, string element)
{
    FunctoidIterator curr;
    FunctoidIterator _end = end();
    for(curr=begin(); curr!=_end; curr++)
        if(((*curr)->GetName()==relation) && (*curr)->IsType(FUNCTOIDRELATION))
            return (*curr)->Get(element);

    return NULL;
}


Functoid* FunctoidNode::Get(uint i)
{
    if(i < size())
        return at(i);
    return NULL;
}


bool FunctoidNode::IsOwner(Functoid* caller)
{
    Functoid* parent = Get(TAG_RELATION_PARENT);
    if(!parent)
        return true;
    if(parent->IsType(TYPE_FUNCTOIDLIST))
    {
        FunctoidList* lparent = (FunctoidList*)parent;
        if((lparent->size()==1) && (lparent->at(0)==caller))
            // This node has only one parent and this parent is the caller
            return true;
    }
    return false;
}


void FunctoidNode::CleanUp()
{
    Functoid* curr;
    // Don't delete the parent(s)
    FunctoidList* parents = dynamic_cast<FunctoidList*>(Get(TAG_RELATION_PARENT));
    if(parents)
        parents->clear();

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


bool FunctoidNode::Detach(Functoid* child)
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



Factory* FunctoidNode::GetFactory()
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


Relation::Relation(string name) : FunctoidList(name)
{
    SetType(FUNCTOIDRELATION);
}


bool Relation::IsOpen(FunctoidSearch* search)
{
    // Does it match in its role as relation
    SearchExpression* se = search->GetSearchRelation();
    if(se && !se->Matches(Name))
    {
        // Don't look further if it's not the right relation
        return false;
    }
    return true;
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


FunctoidType::FunctoidType(string name) : Functoid(name)
{
    SetType(FUNCTOIDTYPE);
}


string FunctoidType::GetAsString()
{
    return Name;
}
