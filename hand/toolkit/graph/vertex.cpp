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


using namespace std;


typedef vector<Vertex*>::iterator VIterator;


Vertex::Vertex(string name)
{
    _Owner = NULL;
    Name(name);
}


Vertex::~Vertex()
{
    VIterator curr = Body.begin();
    while(curr != Body.end())
    {
        // Recursively delete all children
        if(!(*curr)->Owner() || ((*curr)->Owner()==this))
            delete(*curr);
        Body.erase(curr);
    }
}


bool Vertex::Add(Vertex* child)
{
    if(!child)
        return false;
    Body.push_back(child);

    // Take ownership only for unowned objects
    if(!child->Owner())
        child->Owner(this);

    return true;
}


bool Vertex::Set(Vertex* child)
{
    if(!child)
        return false;

    string s = child->Name();
    VIterator curr = Body.begin();
    while(curr != Body.end())
    {
        // TODO: is the Type relevant?
        if((*curr)->Name() == s)
        {
            if(!(*curr)->Owner() || ((*curr)->Owner()==this))
                delete(*curr);
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


Vertex* Vertex::Get()
{
    return _Get();
}


Vertex* Vertex::_Get()
{
    return NULL;
}


Vertex* Vertex::Get(string s)
{
    VIterator curr;
    VIterator _end = Body.end();
    for(curr=Body.begin(); curr!=_end; curr++)
        if((*curr)->Name() == s)
            return (*curr);

    Relation* r = new Relation(s);
    Vertex::Add(r);
    return r;
}


Vertex* Vertex::Get(string type, string name)
{
    VIterator curr = Body.begin();
    VIterator _end = Body.end();

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
            if((*curr)->Name() == name)
                return (*curr);
    }
    else
        for(; curr!=_end; curr++)
            if(((*curr)->Name()==name) && (*curr)->IsType(type))
                return (*curr);

    return NULL;
}


Vertex* Vertex::Get(uint i)
{
    if(i == 0)
        return NULL;

    // 1-based
    --i;

    if(i < Body.size())
        return Body.at(i);

    return NULL;
}


bool Vertex::Delete(Vertex* child)
{
    Vertex::Detach(child);
    if(!child->Owner() || (child->Owner()==this))
    {
        delete(child);
        return true;
    }
    return false;
}


bool Vertex::Detach(Vertex* child)
{
    if(!child)
        return false;

    VIterator curr;
    for(curr=Body.begin(); curr!=Body.end(); curr++)
    {
        if((*curr) == child)
        {
            Body.erase(curr);
            if(child->Owner() == this)
                child->Owner(NULL);
            return true;
        }
    }
    return false;
}


uint Vertex::Size()
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
        if(_Name == name)
            return this;
        return NULL;
    }

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
        if(expression->Matches(ret->Name()))
            return ret;
    }
    return NULL;
}


void Vertex::Name(string name)
{
    _Name = name;
}


string& Vertex::Name()
{
    return _Name;
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
        return types->Body.back()->Name();

    return VERTEX;
}


bool Vertex::IsType(string type)
{
    Vertex* types = Vertex::Get(ANY, TYPE);
    if(!types)
        return (type == VERTEX);

    VIterator _end = types->Body.end();
    for(VIterator curr=types->Body.begin(); curr!=_end; curr++)
        if((*curr)->Name() == type)
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

    VIterator _end = types->Body.end();
    for(VIterator curr=types->Body.begin(); curr!=_end; curr++)
        if(se->Matches((*curr)->Name()))
            return true;

    return false;
}


void Vertex::Owner(Vertex* owner)
{
    _Owner = owner;
}


Vertex* Vertex::Owner()
{
    return _Owner;
}


void Vertex::Reset()
{
    VIterator curr = Body.begin();
    VIterator _end = Body.end();
    while(curr != _end)
    {
        if((*curr)->Owner() == this)
        {
            (*curr)->Vertex::Reset();
            curr++;
        }
        else
        {
            if((*curr)->Owner() == NULL)
                delete(*curr);
            Body.erase(curr);
        }
    }
}


string Vertex::GetAsString()
{
    return _Name;
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
    if(last && (last->Name()==search->GetCookieName()))
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
