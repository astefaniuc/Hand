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
    type(FACTORY);

    if(input_type == "")
        input_type = ANY;
    get(INPUTTYPE)->get(input_type);
    get(REQUEST)->get(output_type);
}


bool Factory::IsValidInput(Vertex* input)
{
    if(!input)
        return false;
    Vertex* itf = get(INPUTTYPE);
    Vertex* it;
    uint i = 0;
    while((it=itf->get(++i)) != NULL)
        if((it->name()==ANY) || input->is(it->name()))
            return true;

    return false;
}


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------


FactoryMap::FactoryMap(string name) : List(name)
{
    type(FACTORYMAP);
}


bool FactoryMap::execute(Vertex* input)
{
    if(!input)
        return NULL;

    Vertex* req = input->Vertex::get(REQUEST);
    if((req->size()==0) || (req->size()>1) || (req->get()->name()==ANY))
        return Resolve(input);

    // TODO: get list of factories for the output type and iterate through it
    // till we get a positive result
    Factory* f = GetFactory(req->get()->name());
    if(!f)
        return false;
    // Factory chain "top down"
    Vertex* it = f->get(INPUTTYPE)->get();
    if(!it || it->name()==ANY || input->is(it->name()))
        // We have the right factory
        return f->execute(input);

    // Change to the subsequent output type
    req->set(it);

    bool ret = execute(input);
    if(ret)
    {
        Vertex* sub_prod = input->get(f->get(REQUEST)->get()->name(), ANY);
        if(sub_prod)
        {
            sub_prod->Vertex::get(REQUEST)->set(req->get());
            // Resolve the intermediate product
            return execute(sub_prod);
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
    input->Vertex::get(REQUEST)->set(f->get(REQUEST)->get());

    return f->execute(input);
}


Factory* FactoryMap::GetFactory(Vertex* target)
{
    Factory* ret = NULL;
    Factory* tmp = NULL;
    uint i = 0;
    Vertex* child;
    while((child=get(++i)) != NULL)
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
    while((child=get(++i)) != NULL)
    {
        ret = dynamic_cast<Factory*>(child);
        if(ret && ret->get(REQUEST)->get(ANY, output_type))
            return ret;
    }

    return NULL;
}
