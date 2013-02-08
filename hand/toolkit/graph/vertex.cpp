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
#include "base/handserver.h"


using namespace std;


Vertex::Vertex(string name)
{
    Type = NULL;
    Name = name;
    Body = new BaseList();
    References = new BaseList();
}


Vertex::~Vertex()
{
    reset();
    delete(Type);
    // Remove references to this object
    for(VIterator curr = References->begin(); curr != References->end(); curr++)
        (*curr)->Body->erase(this);
    delete(References);
    delete(Body);
}


void Vertex::reset()
{
    VIterator curr = Body->begin();
    while(curr != Body->end())
    {
        // Recursively delete all children
        if(!(*curr)->owner() || ((*curr)->owner()==this))
            delete((*curr));
        else
        {
            // Detach
            (*curr)->References->erase(this);
            Body->erase(curr);
        }
    }
}


bool Vertex::add(Vertex* child)
{
    if(!child)
        return false;

    if(Body->find(child)==Body->end())
        Body->push_back(child);
    child->owner(this);

    return true;
}


bool Vertex::set(Vertex* child)
{
    if(!child)
        return false;

    VIterator curr = Body->find(child);
    if(curr == Body->end())
        // New sub-item
        child->References->push_back(this);
    else if(curr == Body->begin())
        // Already set
        return true;
    else
        // Already here but in wrong position
        Body->erase(curr);

    Body->insert(Body->begin(), child);

    return true;
}


bool Vertex::attach(Vertex* child)
{
    if(!child || (Body->find(child)!=Body->end()))
        return false;

    Body->push_back(child);
    child->References->push_back(this);

    return true;
}


Vertex* Vertex::get()
{
    return _get();
}


Vertex* Vertex::_get()
{
    return NULL;
}


Vertex* Vertex::get(string s)
{
    if(s == "")
        return NULL;
    VIterator curr;
    VIterator end = Body->end();
    for(curr=Body->begin(); curr!=end; curr++)
        if((*curr)->name() == s)
            return (*curr);

    Link* r = new Link(s);
    Vertex::add(r);
    return r;
}


Vertex* Vertex::get(string type, string name)
{
    VIterator curr = Body->begin();
    VIterator end = Body->end();

    if(type == ANY)
    {
        for(; curr!=end; curr++)
            if((*curr)->name() == name)
                return (*curr);
    }
    else if(name == ANY)
    {
        for(; curr!=end; curr++)
            if((*curr)->is(type))
                return (*curr);
    }
    else
    {
        for(; curr!=end; curr++)
            if(((*curr)->name()==name) && (*curr)->is(type))
                return (*curr);
        if((type==FACTORY) || (type==FACTORYMAP))
            return HandServer::GetInstance()->get(type, name);
    }

    return NULL;
}


Vertex* Vertex::get(uint i)
{
    if(i == 0)
        return NULL;

    // 1-based
    --i;

    if(i < Body->size())
        return Body->at(i);

    return NULL;
}


bool Vertex::remove(Vertex* child)
{
    if(!child->owner() || (child->owner()==this))
    {
        delete(child);
        return true;
    }
    return Vertex::detach(child);
}


bool Vertex::detach(Vertex* child)
{
    if(!child)
        return false;

    child->References->erase(this);
    return Body->erase(child);
}


uint Vertex::size()
{
    return Body->size();
}


void Vertex::name(string name)
{
    Name = name;
}


string& Vertex::name()
{
    return Name;
}


void Vertex::type(string type)
{
    if(type.empty())
        return;

    if(!Type)
        Type = new Vertex(TYPE);

    Type->set(new Vertex(type));
}


string Vertex::type()
{
    // TODO: needs context sensitive type
    if(Type)
        return Type->Body->front()->name();

    return VERTEX;
}


bool Vertex::is(string type)
{
    if(type.empty())
        return false;

    if(!Type)
        return (type == VERTEX);

    VIterator end = Type->Body->end();
    for(VIterator curr=Type->Body->begin(); curr!=end; curr++)
        if((*curr)->name() == type)
            return true;

    return false;
}


bool Vertex::is(RegularExpression* se)
{
    if(!se)
        return false;

    if(!Type)
        return se->Matches(VERTEX);

    VIterator end = Type->Body->end();
    for(VIterator curr=Type->Body->begin(); curr!=end; curr++)
        if(se->Matches((*curr)->name()))
            return true;

    return false;
}


void Vertex::owner(Vertex* owner)
{
    VIterator curr = References->find(owner);
    if(curr != References->end())
    {
        if(curr == References->begin())
            return;
        References->erase(curr);
    }
    References->insert(References->begin(), owner);
}


Vertex* Vertex::owner()
{
    return References->front();
}


string Vertex::getAsString()
{
    return Name;
}


bool Vertex::execute(Vertex* func_param)
{
    return false;
}


bool Vertex::isOpen(Search* search)
{
    if(Body->size() == 0)
        return true;
    // The search cookie should be the last element
    Vertex* last = Body->back();
    if(last && (last->name()==search->GetCookieName()))
        // Already searched from different branch
        return false;
    return true;
}


Vertex* Vertex::find(string name, int max_depth)
{
    int depth = 0;
    Vertex* result = NULL;
    while(depth <= max_depth)
    {
        // Allow call of overloaded _Find
        result = _find(name, depth);

        if(result)
            break;
        depth++;
    }
    return result;
}


Vertex* Vertex::_find(string name, int depth)
{
    if(depth < 0)
        return NULL;
    if(depth == 0)
    {
        if(Name == name)
            return this;
        return NULL;
    }

    Vertex* ret = NULL;
    uint s = Body->size();
    --depth;
    for(uint i=0; i<s; i++)
    {
        ret = Body->at(i)->Vertex::_find(name, depth);
        if(ret != NULL)
            break;
    }
    return ret;
}


Vertex* Vertex::find(RegularExpression* expression)
{
    // "Plain" find, don't descend
    uint s = Body->size();
    Vertex* ret;
    for(uint i=0; i<s; i++)
    {
        ret = Body->at(i);
        if(expression->Matches(ret->name()))
            return ret;
    }
    return NULL;
}
