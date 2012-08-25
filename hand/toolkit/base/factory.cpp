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

#include "base/factory.h"
#include "graph/data.h"


using namespace std;


Factory::Factory
(
        string name,
        string input_type,
        string output_type,
        string uri_scheme
) : HandApp(name)
{
    SetType(FACTORY);
    SetSpecifierString(FACTORY_INPUTSTRING, input_type);
    SetSpecifierString(FACTORY_OUTPUTSTRING, output_type);
    SetSpecifierString(URI_FACTORY, uri_scheme);
}


string Factory::GetInputType()
{
    return GetSpecifierString(FACTORY_INPUTSTRING);
}


string Factory::GetOutputType()
{
    return GetSpecifierString(FACTORY_OUTPUTSTRING);
}


string Factory::GetUriScheme()
{
    return GetSpecifierString(URI_FACTORY);
}


bool Factory::SetSpecifierString(string name, string value)
{
    if(value == "")
        return false;
    Add(new Note(name, value));
    return true;
}


string Factory::GetSpecifierString(string name)
{
    Note* d = dynamic_cast<Note*>(Get(name));
    if(d)
        return d->Get();
    return "";
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


FactoryMap::FactoryMap(string name) : List(name)
{
    SetType(FACTORYMAP);
}


Vertex* FactoryMap::Produce(Vertex* entry, string output_type)
{
    if(!entry)
        return NULL;

    Factory* f = GetFactory(output_type);
    if(f)
    {
        if(f->IsValidInput(entry))
            // We have the right factory
            return f->Produce(entry);
        else
            // Factory chain "top down"
            return Produce(Produce(entry, f->GetInputType()), output_type);
    }

    // Factory chain "bottom up"
    Vertex* ret = entry;

    f = GetFactory(entry);
    if(f)
    {
        ret = f->Produce(entry);
        if(ret)
        {
            // Do we have a new entry or a new factory?
            f = dynamic_cast<Factory*>(ret);
            if(f)
                Add(f);
            else
                entry = ret;
            return Produce(entry, output_type);
        }
        else
            ret = entry;
    }
    if(output_type == "")
        // Can't continue, return what we have so far
        return ret;
    return NULL;
}


Factory* FactoryMap::GetFactory(Vertex* target)
{
    Factory* ret = NULL;
    Factory* tmp = NULL;
    uint i = 0;
    Vertex* child;
    while((child=Get(++i)) != NULL)
    {
        tmp = dynamic_cast<Factory*>(child);
        // Find factory with the biggest relevance, for now it is the last added one
        // TODO: better
        if(tmp && tmp->IsValidInput(target))
            ret = tmp;
    }

    return ret;
}


Factory* FactoryMap::GetFactory(string output_type)
{
    if(output_type == "")
        return NULL;

    Factory* ret;
    uint i = 0;
    Vertex* child;
    while((child=Get(++i)) != NULL)
    {
        ret = dynamic_cast<Factory*>(child);
        if(ret && (ret->GetOutputType()==output_type))
            return ret;
    }

    return NULL;
}
