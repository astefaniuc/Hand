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
#include "graph/search/regularexpression.h"
#include "graph/search/search.h"

#include <typeinfo>


using namespace std;


typedef vector<Vertex*>::iterator VertexIterator;


Vertex::Vertex(string name)
{
    SetName(name);
}


Vertex::~Vertex()
{
    // Don't delete the parent(s)
    Vertex* curr = Get(RELATION, OWNER);
    // Reset() doesn't work here
    if(curr && (curr->Body.size()== 2))
        curr->Body.pop_back();

    uint s = Body.size();
    for(uint i=0; i<s; i++)
    {
        curr = Body.at(i);

        if(curr->HasOwner(this))
        {
            // Recursively delete all children
            delete curr;
        }
    }
    if(s != 0)
        Body.clear();
}


bool Vertex::Add(Vertex* child)
{
    if(!child)
        return false;
    Body.push_back(child);
    // Take ownership only for unowned objects
    if(child->HasOwner(this))
        child->SetOwner(this);

    return true;
}


bool Vertex::Set(Vertex* child)
{
    if(!child)
        return false;

    string s = child->GetName();
    VertexIterator curr = Body.begin();
    while(curr != Body.end())
    {
        // TODO: is Type relevant?
        if((*curr)->GetName() == s)
        {
            if((*curr)->HasOwner(this))
                delete((*curr));
            Body.erase(curr);
        }
        else
            curr++;
    }
    Body.push_back(child);
    return true;
}


bool Vertex::Attach(Vertex* child)
{
    if(!child)
        return false;
    Body.push_back(child);
    return true;
}


Vertex* Vertex::Get(string s)
{
    VertexIterator curr;
    VertexIterator _end = Body.end();
    for(curr=Body.begin(); curr!=_end; curr++)
        if((*curr)->GetName() == s)
            return (*curr);

    Relation* r = new Relation(s);
    Vertex::Add(r);
    return r;
}


Vertex* Vertex::Get(string type, string name)
{
    VertexIterator curr = Body.begin();
    VertexIterator _end = Body.end();

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


Vertex* Vertex::Get(uint i)
{
    // 1-based
    if(i == 0)
        //return NULL;
        exit(666);
    --i;
    if(i < Body.size())
        return Body.at(i);
    return NULL;
}


bool Vertex::Delete(Vertex* child)
{
    Vertex::Detach(child);
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
    for(curr=Body.begin(); curr!=Body.end(); curr++)
    {
        if((*curr) == child)
        {
            Body.erase(curr);
            return true;
        }
    }
    return false;
}


uint Vertex::GetSize()
{
    return Body.size();
}


Vertex* Vertex::Find(string name, int max_depth)
{
    int depth = 0;
    Vertex* result = NULL;
    while(depth <= max_depth)
    {
        // Allow call of overloaded _Find
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
    uint s = Body.size();
    --depth;
    for(uint i=0; i<s; i++)
    {
        ret = Body.at(i)->Vertex::_Find(name, depth);
        if(ret != NULL)
            break;
    }
    return ret;
}


Vertex* Vertex::Find(RegularExpression* expression)
{
    // "Plain" find, don't descend
    uint s = Body.size();
    Vertex* ret;
    for(uint i=0; i<s; i++)
    {
        ret = Body.at(i);
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
    return (GetType() + Name);
}


void Vertex::SetType(string type)
{
    if(type.empty())
        return;
    Vertex* types = Vertex::Get(ANY, TYPE);
    if(!types)
    {
        types = new Vertex(TYPE);
        Vertex::Set(types);
    }
    types->Vertex::Set(new Vertex(type));
}


string Vertex::GetType()
{
    // TODO: needs context sensitive type
    Vertex* types = Vertex::Get(ANY, TYPE);
    if(types)
        return types->Body.back()->GetName();

    return VERTEX;
}


bool Vertex::IsType(string type)
{
    Vertex* types = Vertex::Get(ANY, TYPE);
    if(!types)
        return (type == VERTEX);

    VertexIterator _end = types->Body.end();
    for(VertexIterator curr=types->Body.begin(); curr!=_end; curr++)
        if((*curr)->GetName() == type)
            return true;

    return false;
}


bool Vertex::IsType(RegularExpression* se)
{
    if(!se)
        return false;

    Vertex* types = Vertex::Get(ANY, TYPE);
    if(!types)
        return se->Matches(VERTEX);

    VertexIterator _end = types->Body.end();
    for(VertexIterator curr=types->Body.begin(); curr!=_end; curr++)
        if(se->Matches((*curr)->GetName()))
            return true;

    return false;
}


void Vertex::SetOwner(Vertex* owner)
{
    Relation* r = new Relation(OWNER);
    Vertex::Attach(r);
    r->Set(owner);
}


bool Vertex::HasOwner(Vertex* caller)
{
    Vertex* rel_p = Vertex::Get(RELATION, OWNER);
    if(!rel_p)
        return true;
    uint ps = rel_p->Body.size();
    // Check if this node has only the caller as parent
    return ((ps < 2) || ((ps == 2) && (rel_p->Body.at(1) == caller)));
}


void Vertex::Reset()
{
    VertexIterator curr = Body.begin();
    VertexIterator _end = Body.end();
    while(curr != _end)
    {
        // TODO: deleted also attached objects, needs fix in List
        if((*curr)->HasOwner(this))
        {
            (*curr)->Vertex::Reset();
            curr++;
        }
        else
            Body.erase(curr);
    }
}


string Vertex::GetAsString()
{
    return Name;
}


bool Vertex::Execute(Vertex* func_param)
{
    return false;
}


bool Vertex::IsOpen(Search* search)
{
    if(Body.size() == 0)
        return true;
    // The search cookie should be the last element
    Vertex* last = Body.back();
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
