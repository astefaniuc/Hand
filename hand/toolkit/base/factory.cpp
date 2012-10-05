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
) : List(name)
{
    Type(FACTORY);
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
    Type(FACTORYMAP);
}


bool FactoryMap::Execute(Vertex* input)
{
    if(!input)
        return NULL;

    string output_type = "";
    Vertex* ot = input->Vertex::Get("Output Type")->Get();
    if(ot)
        output_type = ot->Name();

    // TODO: get list of factories for the output type and iterate through it
    // till we get a positive result
    Factory* f = GetFactory(output_type);
    if(f)
    {
        // Factory chain "top down"
        if(f->IsValidInput(input))
            // We have the right factory
            return f->Execute(input);

        // Change to the subsequent output type
        input->Vertex::Get("Output Type")->Set(new Vertex(f->GetOutputType()));
        if(Execute(input))
            // Resolve the intermediate product
            return Execute(input->Get(f->GetOutputType(), ANY));
        return false;
    }

    // Factory chain "bottom up"
    f = GetFactory(input);
    if(!f)
        return false;

    // Set output type for caller
    input->Vertex::Get("Output Type")->Set(new Vertex(f->GetOutputType()));

    return f->Execute(input);
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
