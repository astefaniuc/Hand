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


Factory::Factory(
        string name,
        string input_type,
        string output_type
) : Vertex(name)
{
    Type(FACTORY);

    if(input_type == "")
        input_type = ANY;
    Get(INPUTTYPE)->Set(new Vertex(input_type));
    Get(OUTPUTTYPE)->Set(new Vertex(output_type));
}


bool Factory::IsValidInput(Vertex* input)
{
    if(!input)
        return false;
    Vertex* itf = Get(INPUTTYPE);
    Vertex* it;
    uint i = 0;
    while((it=itf->Get(++i)) != NULL)
        if((it->Name()==ANY) || input->Is(it->Name()))
            return true;

    return false;
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

    Vertex* ot = input->Vertex::Get(OUTPUTTYPE)->Get();
    if(!ot)
        return false;
    if(ot->Name() == ANY)
        return Resolve(input);

    // TODO: get list of factories for the output type and iterate through it
    // till we get a positive result
    Factory* f = GetFactory(ot->Name());
    if(!f)
        return false;
    // Factory chain "top down"
    if(f->IsValidInput(input))
        // We have the right factory
        return f->Execute(input);

    // Change to the subsequent output type
    input->Vertex::Get(OUTPUTTYPE)->Set(f->Get(INPUTTYPE)->Get());

    bool ret = Execute(input);
    if(ret)
    {
        Vertex* sub_prod = input->Get(f->Get(OUTPUTTYPE)->Get()->Name(), ANY);
        if(sub_prod)
        {
            sub_prod->Vertex::Get(OUTPUTTYPE)->Set(ot);
            // Resolve the intermediate product
            return Execute(sub_prod);
        }
    }
    return false;
}


bool FactoryMap::Resolve(Vertex* input)
{
    // Factory chain "bottom up"
    Vertex* f = GetFactory(input);
    if(!f)
        return false;

    // Set output type for caller
    input->Vertex::Get(OUTPUTTYPE)->Set(f->Get(OUTPUTTYPE)->Get());

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
        if(ret && ret->Get(OUTPUTTYPE)->Get(ANY, output_type))
            return ret;
    }

    return NULL;
}
