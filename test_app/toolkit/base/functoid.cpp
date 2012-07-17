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
#include "functoidlist.h"
#include "searchexpression.h"

#include <typeinfo>


using namespace std;


Functoid::Functoid(string name)
{
    SetName(name);
    SetType("Functoid");
}


Functoid* Functoid::Find(string name, int max_depth)
{
    return _Find(name, max_depth);
}


Functoid* Functoid::_Find(string name, int depth)
{
    if((depth==0) && (Name==name))
        return this;
    return NULL;
}


bool Functoid::IsList()
{
    return false;
}


bool Functoid::IsOpen(FunctoidSearch* search)
{
    return true;
}


bool Functoid::Add(Functoid* ignore)
{
    // Only FunctoidList can do this
    return false;
}


bool Functoid::Set(Functoid* ignore)
{
    // Only FunctoidList can do this
    return false;
}


Functoid* Functoid::Get(string s)
{
    // No children in the base class
    return NULL;
}


Functoid* Functoid::Get(uint child)
{
    // No children in the base class
    return NULL;
}


bool Functoid::Delete(Functoid* ignore)
{
    // Only FunctoidList can do this
    return false;
}


bool Functoid::Attach(Functoid* ignore)
{
    // Only FunctoidList and above can do this
    return false;
}


bool Functoid::Detach(Functoid* child)
{
    // Only FunctoidList can do this
    return false;
}


void Functoid::SetType(string type)
{
    if(type != "")
        Type = type;
}


string Functoid::GetType()
{
    return Type;
}


bool Functoid::IsType(string type)
{
    if(Type == type)
        return true;
    return false;
}


bool Functoid::IsType(SearchExpression* type)
{
    if(type && type->Matches(Type))
        return true;
    return false;
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
    {
        IsMulti = true;
    }
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
        FunctoidList* new_val = new FunctoidList("Value");
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
