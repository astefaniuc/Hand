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
    CleanUp();
}


void Functoid::CleanUp()
{
    uint s = size();
    for(uint i=0; i<s; i++)
        // Recursivelly delete all childrens:
        delete at(i);
    if(s != 0)
        clear();
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


bool Functoid::IsOpen(FunctoidSearch* search)
{
    // The search cookie should be the last element
    Functoid* last = back();
    if(last && (last->GetName()==search->GetCookieName()))
        // Already searched from different branch
        return false;
    return true;
}


bool Functoid::Add(Functoid* child)
{
    if(!child)
        return false;
    push_back(child);
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
            delete(*curr);
            erase(curr);
        }
        else
            curr++;
    }
    push_back(child);
    return true;
}


Functoid* Functoid::Get(string s)
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


Functoid* Functoid::Get(uint child)
{
    // No children in the base class
    return NULL;
}


bool Functoid::Delete(Functoid* child)
{
    if(!Detach(child))
        return false;

    delete(child);
    return true;
}


bool Functoid::Attach(Functoid* child)
{
    Add(child);
    return true;
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


void Functoid::SetType(string type)
{
    if(type != "")
        Set(new Note(TAG_TYPE, type));
}


string Functoid::GetType()
{
    Functoid* ret = Get(TAG_TYPE);
    if(ret)
        return ret->GetName();
    return "Functoid";
}


bool Functoid::IsType(string type)
{
    Functoid* ret = Get(TAG_TYPE);
    if(ret)
        return (ret->GetName() == type);
    return false;
}


bool Functoid::IsType(SearchExpression* s_type)
{
    if(!s_type)
        return false;
    Functoid* type = Get(TAG_TYPE);
    if(!type)
        return false;
    return s_type->Matches(type->GetName());
}


bool Functoid::Execute(Functoid* func_param)
{
    return false;
}


bool Functoid::SetOwner(Functoid* ignore)
{
    return false;
}


Functoid* Functoid::GetOwner()
{
    return false;
}


bool Functoid::IsOwner(Functoid* ignore)
{
    // Data and Callback Functoids are always directly tied to only one ListFunctoid
    // thus can be deleted from there
    return true;
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

// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


Link::Link(string name, string type, uint size) : Functoid(name)
{
    Value = NULL;
    SetType(type);
    IsMulti = false;
    if(size > 1)
        IsMulti = true;
}


Link::~Link()
{
    // Is owner
    delete(Value);
}


Functoid* Link::Find(string name, int max_depth)
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


Functoid* Link::_Find(string name, int depth)
{
    if(depth <= 0)
    {
        if(Name == name)
            return this;
    }
    else if(Value)
        return Value->_Find(name, --depth);
    return NULL;
}


bool Link::Set(Functoid* val)
{
    if(IsMulti)
        return Value->Set(val);
    Value = val;
    return true;
}


Functoid* Link::Get(uint i)
{
    if(IsMulti)
        return Value->Get(i);
    if(i == 1)
        return Value;
    return NULL;
}


bool Link::Add(Functoid* val)
{
    if(IsMulti)
        return Value->Add(val);
    if(Value)
        return false;
    Value = val;
    return true;
}


Functoid* Link::Get()
{
    return Value;
}


string Link::GetAsString()
{
    return GetName() + "/" + GetType();
}


bool Link::Execute(Functoid* vs)
{
    if(Value)
        return Value->Execute(vs);
    return false;
}


bool Link::IsList()
{
    return true;
}


void Link::MakeMultiLink(bool cond)
{
    if(IsMulti == cond)
        return;
    if(IsMulti)
    {
        // TODO: delete or don't the linked elements?
        Value = Value->Get(1);
    }
    else
    {
        FunctoidNode* new_val = new FunctoidNode("Value");
        if(Value)
            // Rescue old value
            new_val->Add(Value);
        Value = new_val;
    }
    IsMulti = cond;
}


bool Link::IsMultiLink()
{
    return IsMulti;
}
