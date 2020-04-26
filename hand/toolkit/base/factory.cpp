#include "factory.h"
#include "graph/data.h"


Factory::Factory(
    const std::string& name, const std::string& input_type, const std::string& output_type)
    : Vertex(name)
{
    type(FACTORY);

    if(input_type.empty())
        get(INPUTTYPE)->get(ANY);
    else
        get(INPUTTYPE)->get(input_type);

    get(OUTPUTTYPE)->get(output_type);
}


bool Factory::IsValidInput(Vertex* input)
{
    if(!input)
        return false;

    Vertex* itf = get(INPUTTYPE);
    Vertex* it;
    uint i = 0;
    while((it=itf->get(++i)) != nullptr)
        if((it->name() == ANY) || input->is(it->name()))
            return true;

    return false;
}


// ----------------------------------------------------------------


FactoryMap::FactoryMap(const std::string& name) : List(name)
{
    type(FACTORYMAP);
}


bool FactoryMap::execute(Vertex* input)
{
    if(!input)
        return false;

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
        Vertex* sub_prod = input->get(f->get(OUTPUTTYPE)->get()->name(), ANY);
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
    input->Vertex::get(REQUEST)->set(f->get(OUTPUTTYPE)->get());

    return f->execute(input);
}


Factory* FactoryMap::GetFactory(Vertex* target)
{
    Factory* ret = nullptr;
    Factory* tmp = nullptr;
    uint i = 0;
    Vertex* child;
    while((child=get(++i)) != nullptr)
    {
        tmp = dynamic_cast<Factory*>(child);
        // Find factory with the biggest relevance, for now it is the last added one
        // TODO: better
        if(tmp && tmp->IsValidInput(target))
            ret = tmp;
    }

    return ret;
}


Factory* FactoryMap::GetFactory(const std::string& output_type)
{
    if(output_type.empty())
        return nullptr;

    Factory* ret;
    uint i = 0;
    Vertex* child;
    while((child=get(++i)) != nullptr)
    {
        ret = dynamic_cast<Factory*>(child);
        if(ret && ret->get(OUTPUTTYPE)->get(ANY, output_type))
            return ret;
    }

    return nullptr;
}
