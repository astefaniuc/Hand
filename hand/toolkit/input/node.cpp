#include "input/node.h"


StateNode::StateNode(uint size, Vertex* peers) : List(STATENODE)
{
    type(STATENODE);
    // Init the related Nodes container with "empty" objects
    for(uint i=1; i<=size; i++)
        add(new List(CHILD));

    Vertex::attach(peers);
    // Adds itself to the peers list
    peers->add(this);
}


bool StateNode::set(Vertex* sub)
{
    if(!sub || !sub->is(METHOD) || Vertex::get(METHOD, ANY))
        return false;
    return Vertex::set(sub);
}


Vertex* StateNode::get(std::string name)
{
    Vertex* ret = List::get(ANY, name);
    if(ret || (name!=VIEW))
        return ret;

    Vertex* keylist = Vertex::get(COMMANDS)->Vertex::get(DEVICE, ANY)->get(VIEW)->get(KEYLIST);

    ret = List::get(VIEW);
    ret->Vertex::set(keylist->Vertex::get(LAYOUT));

    Vertex* active_keys;
    uint i = 0;
    while((active_keys=get(++i)) != nullptr)
        if(active_keys->name() == PARENT)
             ret->attach(keylist->get(i));

    return ret;
}


StateNode* StateNode::GetParent(uint k_nr)
{
    Vertex* ret = get(k_nr);
    if(ret && (ret->name() == PARENT))
        return dynamic_cast<StateNode*>(ret->get());

    return nullptr;
}


StateNode* StateNode::GetChild(uint k_nr)
{
    Vertex* ret = get(k_nr);
    if(ret && (ret->name() == CHILD))
        return dynamic_cast<StateNode*>(ret->get());

    return nullptr;
}
