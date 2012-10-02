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

#include "graph/baselist.h"
#include "graph/vertex.h"
#include "graph/link.h"
#include "graph/search/regularexpression.h"
#include "graph/search/search.h"


using namespace std;


Vertex::Vertex(string name)
{
    _Type = NULL;
    _Name = name;
    _Body = new BaseList();
    _References = new BaseList();
}


Vertex::~Vertex()
{
    Reset();
    delete(_Type);
    // Remove references to this object
    for(VIterator curr = _References->begin(); curr != _References->end(); curr++)
        (*curr)->_Body->erase(this);
    delete(_References);
    delete(_Body);
}


void Vertex::Reset()
{
    VIterator curr = _Body->begin();
    while(curr != _Body->end())
    {
        // Recursively delete all children
        if(!(*curr)->Owner() || ((*curr)->Owner()==this))
            delete((*curr));
        else
        {
            // Detach
            (*curr)->_References->erase(this);
            _Body->erase(curr);
        }
    }
}


bool Vertex::Add(Vertex* child)
{
    if(!child)
        return false;

    _Body->push_back(child);
    child->Owner(this);

    return true;
}


bool Vertex::Set(Vertex* child)
{
    if(!child)
        return false;

    string s = child->Name();
    VIterator curr = _Body->begin();
    while(curr != _Body->end())
    {
        // TODO: is the Type relevant?
        if((*curr)->Name() == s)
        {
            if(!(*curr)->Owner() || ((*curr)->Owner()==this))
                delete(*curr);
            else
                _Body->erase(curr);
        }
        else
            curr++;
    }
    _Body->push_back(child);
    child->_References->push_back(this);

    return true;
}


bool Vertex::Attach(Vertex* child)
{
    if(!child)
        return false;

    _Body->push_back(child);
    child->_References->push_back(this);

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
    VIterator _end = _Body->end();
    for(curr=_Body->begin(); curr!=_end; curr++)
        if((*curr)->Name() == s)
            return (*curr);

    Relation* r = new Relation(s);
    Vertex::Add(r);
    return r;
}


Vertex* Vertex::Get(string type, string name)
{
    VIterator curr = _Body->begin();
    VIterator _end = _Body->end();

    if(name == ANY)
    {
        if((type==ANY) && (curr!=_end))
            return (*curr);
        else
            for(; curr!=_end; curr++)
                if((*curr)->Is(type))
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
            if(((*curr)->Name()==name) && (*curr)->Is(type))
                return (*curr);

    return NULL;
}


Vertex* Vertex::Get(uint i)
{
    if(i == 0)
        return NULL;

    // 1-based
    --i;

    if(i < _Body->size())
        return _Body->at(i);

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

    child->_References->erase(this);
    return _Body->erase(child);
}


uint Vertex::Size()
{
    return _Body->size();
}


void Vertex::Name(string name)
{
    _Name = name;
}


string& Vertex::Name()
{
    return _Name;
}


void Vertex::Type(string type)
{
    if(type.empty())
        return;

    if(!_Type)
        _Type = new Vertex(TYPE);

    _Type->Set(new Vertex(type));
}


string Vertex::Type()
{
    // TODO: needs context sensitive type
    if(_Type)
        return _Type->_Body->back()->Name();

    return VERTEX;
}


bool Vertex::Is(string type)
{
    if(type.empty())
        return false;

    if(!_Type)
        return (type == VERTEX);

    VIterator _end = _Type->_Body->end();
    for(VIterator curr=_Type->_Body->begin(); curr!=_end; curr++)
        if((*curr)->Name() == type)
            return true;

    return false;
}


bool Vertex::Is(RegularExpression* se)
{
    if(!se)
        return false;

    if(!_Type)
        return se->Matches(VERTEX);

    VIterator _end = _Type->_Body->end();
    for(VIterator curr=_Type->_Body->begin(); curr!=_end; curr++)
        if(se->Matches((*curr)->Name()))
            return true;

    return false;
}


void Vertex::Owner(Vertex* owner)
{
    _References->insert(_References->begin(), owner);
}


Vertex* Vertex::Owner()
{
    return _References->front();
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
    if(_Body->size() == 0)
        return true;
    // The search cookie should be the last element
    Vertex* last = _Body->back();
    if(last && (last->Name()==search->GetCookieName()))
        // Already searched from different branch
        return false;
    return true;
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
    uint s = _Body->size();
    --depth;
    for(uint i=0; i<s; i++)
    {
        ret = _Body->at(i)->Vertex::_Find(name, depth);
        if(ret != NULL)
            break;
    }
    return ret;
}


Vertex* Vertex::Find(RegularExpression* expression)
{
    // "Plain" find, don't descend
    uint s = _Body->size();
    Vertex* ret;
    for(uint i=0; i<s; i++)
    {
        ret = _Body->at(i);
        if(expression->Matches(ret->Name()))
            return ret;
    }
    return NULL;
}
