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

#include "graph/vertex.h"
#include "graph/link.h"
#include "graph/search/searchexpression.h"
#include "graph/search/functoidsearch.h"

#include <typeinfo>


using namespace std;


Vertex::Vertex(string name)
{
    SetName(name);
}


Vertex::~Vertex()
{
    // Don't delete the parent(s)
    Vertex* curr = Get(RELATION, OWNER);
    // Reset() doesn't work here
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


bool Vertex::Add(Vertex* child)
{
    if(!child)
        return false;
    push_back(child);
    child->SetOwner(this);
    return true;
}


bool Vertex::Set(Vertex* child)
{
    if(!child)
        return false;

    string s = child->GetName();
    VertexIterator curr = begin();
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


bool Vertex::Attach(Vertex* child)
{
    if(!child)
        return false;
    push_back(child);
    return true;
}


Vertex* Vertex::Get(string s)
{
    VertexIterator curr;
    VertexIterator _end = end();
    for(curr=begin(); curr!=_end; curr++)
        if((*curr)->GetName() == s)
            return (*curr);

    Relation* r = new Relation(s);
    Add(r);
    return r;
}


Vertex* Vertex::Get(string type, string name)
{
    VertexIterator curr = begin();
    VertexIterator _end = end();

    if(name == ANY)
    {
        if((type==ANY) && (curr!=_end))
            return (*curr);
        else
            for(; curr!=_end; curr++)
                if((*curr)->IsType(type))
                    return (*curr);
    }
    else if(type == ANY)
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


Vertex* Vertex::Get(uint child)
{
    // Reserved for the List class
    return NULL;
}


bool Vertex::Delete(Vertex* child)
{
    Detach(child);
    if(child->HasOwner(this))
    {
        delete(child);
        return true;
    }
    return false;
}


bool Vertex::Detach(Vertex* child)
{
    if(!child) return false;

    VertexIterator curr;
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


Vertex* Vertex::Find(string name, int max_depth)
{
    int depth = 0;
    Vertex* result = NULL;
    while(depth <= max_depth)
    {
        result = _Find(name, depth);

        if(result)
            break;
        depth++;
    }
    return result;
}


Vertex* Vertex::_Find(string name, int depth)
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
    if(Name == OWNER)
        return NULL;

    Vertex* ret = NULL;
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


Vertex* Vertex::Find(SearchExpression* expression)
{
    // "Plain" find, don't descend
    uint s = size();
    Vertex* ret;
    for(uint i=0; i<s; i++)
    {
        ret = at(i);
        if(expression->Matches(ret->GetName()))
            return ret;
    }
    return NULL;
}


void Vertex::SetName(string name)
{
    Name = name;
}


string& Vertex::GetName()
{
    return Name;
}


string Vertex::GetUriString()
{
    // TODO: needs context sensitive type
    return (GetType() + Name);
}


void Vertex::SetType(string type)
{
    if(type.empty())
        return;
    Vertex* types = Get(ANY, TYPE);
    if(!types)
    {
        types = new Vertex(TYPE);
        Set(types);
    }
    types->Set(new Vertex(type));
}


string Vertex::GetType()
{
    Vertex* types = Get(TYPE);
    if(types)
        return types->back()->GetName();
    return "Vertex";
}


bool Vertex::IsType(string type)
{
    Vertex* types = Get(TYPE);
    VertexIterator _end = types->end();
    for(VertexIterator curr=types->begin(); curr!=_end; curr++)
        if((*curr)->GetName() == type)
            return true;

    return false;
}


bool Vertex::IsType(SearchExpression* se)
{
    if(!se)
        return false;
    Vertex* types = Get(TYPE);
    VertexIterator _end = types->end();
    for(VertexIterator curr=types->begin(); curr!=_end; curr++)
        if(se->Matches((*curr)->GetName()))
            return true;

    return false;
}


void Vertex::SetOwner(Vertex* owner)
{
    Relation* r = new Relation(OWNER);
    Attach(r);
    r->Set(owner);
}


bool Vertex::HasOwner(Vertex* caller)
{
    Vertex* rel_p = Get(RELATION, OWNER);
    if(!rel_p)
        return true;
    uint ps = rel_p->size();
    // Check if this node has only the caller as parent
    return ((ps < 2) || ((ps == 2) && (rel_p->at(1) == caller)));
}


bool Vertex::Execute(Vertex* func_param)
{
    return false;
}


bool Vertex::IsOpen(VertexSearch* search)
{
    // The search cookie should be the last element
    Vertex* last = back();
    if(last && (last->GetName()==search->GetCookieName()))
        // Already searched from different branch
        return false;
    return true;
}

/*
bool Vertex::NotifyChanged()
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
