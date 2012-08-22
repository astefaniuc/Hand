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

#include "functoid.h"
#include "searchexpression.h"
#include "functoidsearch.h"

#include <typeinfo>


using namespace std;


Functoid::Functoid(string name)
{
    SetName(name);
}


Functoid::~Functoid()
{
    // Don't delete the parent(s)
    Functoid* curr = Get(FUNCTOIDRELATION, TAG_RELATION_PARENT);
    // CleanUp() doesn't work here
    if(curr && (curr->size()== 2))
        curr->pop_back();

    uint s = size();
    for(uint i=0; i<s; i++)
    {
        curr = at(i);

        if(curr->HasOwner(this))
        {
            // Recursively delete all children
            delete curr;
        }
    }
    if(s != 0)
        clear();
}


bool Functoid::Add(Functoid* child)
{
    if(!child)
        return false;
    push_back(child);
    child->SetOwner(this);
    return true;
}


bool Functoid::Set(Functoid* child)
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
            if((*curr)->HasOwner(this))
                delete((*curr));
            erase(curr);
        }
        else
            curr++;
    }
    push_back(child);
    return true;
}


bool Functoid::Attach(Functoid* child)
{
    if(!child)
        return false;
    push_back(child);
    return true;
}


Functoid* Functoid::Get(string s)
{
    FunctoidIterator curr;
    FunctoidIterator _end = end();
    for(curr=begin(); curr!=_end; curr++)
        if((*curr)->GetName() == s)
            return (*curr);

    Relation* r = new Relation(s);
    Add(r);
    return r;
}


Functoid* Functoid::Get(string type, string name)
{
    FunctoidIterator curr = begin();
    FunctoidIterator _end = end();

    if(name == IGNORE)
    {
        if((type==IGNORE) && (curr!=_end))
            return (*curr);
        else
            for(; curr!=_end; curr++)
                if((*curr)->IsType(type))
                    return (*curr);
    }
    else if(type == IGNORE)
    {
        for(; curr!=_end; curr++)
            if((*curr)->GetName()==name)
                return (*curr);
    }
    else
        for(; curr!=_end; curr++)
            if(((*curr)->GetName()==name) && (*curr)->IsType(type))
                return (*curr);

    return NULL;
}


Functoid* Functoid::Get(uint child)
{
    // Reserved for the FunctoidList class
    return NULL;
}


bool Functoid::Delete(Functoid* child)
{
    Detach(child);
    if(child->HasOwner(this))
    {
        delete(child);
        return true;
    }
    return false;
}


bool Functoid::Detach(Functoid* child)
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


Functoid* Functoid::Find(string name, int max_depth)
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


Functoid* Functoid::_Find(string name, int depth)
{
    if(depth < 0)
        return NULL;
    if(depth == 0)
    {
        if(Name == name)
            return this;
        return NULL;
    }
    // Don't search parent/owner to avoid a sure graph cycle
    if(Name == TAG_RELATION_PARENT)
        return NULL;

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


Functoid* Functoid::Find(SearchExpression* expression)
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


void Functoid::SetName(string name)
{
    Name = name;
}


string& Functoid::GetName()
{
    return Name;
}


string Functoid::GetUriString()
{
    // TODO: needs context sensitive type
    return (GetType() + Name);
}


void Functoid::SetType(string type)
{
    if(type.empty())
        return;
    Functoid* types = Get(IGNORE, TAG_TYPE);
    if(!types)
    {
        types = new Functoid(TAG_TYPE);
        Set(types);
    }
    types->Set(new Functoid(type));
}


string Functoid::GetType()
{
    Functoid* types = Get(TAG_TYPE);
    if(types)
        return types->back()->GetName();
    return "Functoid";
}


bool Functoid::IsType(string type)
{
    Functoid* types = Get(TAG_TYPE);
    FunctoidIterator _end = types->end();
    for(FunctoidIterator curr=types->begin(); curr!=_end; curr++)
        if((*curr)->GetName() == type)
            return true;

    return false;
}


bool Functoid::IsType(SearchExpression* se)
{
    if(!se)
        return false;
    Functoid* types = Get(TAG_TYPE);
    FunctoidIterator _end = types->end();
    for(FunctoidIterator curr=types->begin(); curr!=_end; curr++)
        if(se->Matches((*curr)->GetName()))
            return true;

    return false;
}


void Functoid::SetOwner(Functoid* owner)
{
    Relation* r = new Relation(TAG_RELATION_PARENT);
    Attach(r);
    r->Set(owner);
}


bool Functoid::HasOwner(Functoid* caller)
{
    Functoid* rel_p = Get(FUNCTOIDRELATION, TAG_RELATION_PARENT);
    if(!rel_p)
        return true;
    uint ps = rel_p->size();
    // Check if this node has only the caller as parent
    return ((ps < 2) || ((ps == 2) && (rel_p->at(1) == caller)));
}


bool Functoid::Execute(Functoid* func_param)
{
    return false;
}


bool Functoid::IsOpen(FunctoidSearch* search)
{
    // The search cookie should be the last element
    Functoid* last = back();
    if(last && (last->GetName()==search->GetCookieName()))
        // Already searched from different branch
        return false;
    return true;
}

/*
bool Functoid::NotifyChanged()
{
    // TODO: insert a "Changed" flag under "properties"
    Layer* layer;
    if((layer = GetLayer()) != NULL)
    {
        layer->NotifyChanged();
        return true;
    }
    return false;

}
*/
