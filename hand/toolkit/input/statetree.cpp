#include "input/statetree.h"
#include "input/node.h"


StateGraph::StateGraph() : Vertex(STATEGRAPH)
{
    type(FACTORY);
}


bool StateGraph::execute(Vertex* device)
{
    unsigned size = device->Vertex::get(KEYLIST)->size();
    if(!size || (size > MAX_NUMBER_OF_BUTTONS))
        return false;
    set(device);
    // Create the root/null key
    Root = new StateNode(size, GetNewPeersList());
    // Build the key tree on top
    for(unsigned i = 1; i <= size; ++i)
        // Build the key tree bellow
        AddNodes(1, i);

    device->Vertex::add(Root);
    return true;
}


void StateGraph::AddNodes(uint level, uint key_nr)
{
    Vertex* tmp_node;
    Vertex* new_node;

    // Previous level nodes
    Vertex* p_nodes = GetCommands(level-1);
    // Current level nodes
    Vertex* c_nodes = GetCommands(level);

    uint i = 0;
    Vertex* parent_node;
    // Create a new node for each one existing one level down
    while((parent_node=p_nodes->get(++i)) != nullptr)
    {
        // Ignore nodes inserted for the current key
        if(parent_node->get(key_nr)->get())
            continue;

        // Appends also the new node to c_nodes
        new_node = new StateNode(Root->size(), c_nodes);
        // Connect new and old node at key number position
        ConnectNodes(parent_node, new_node, key_nr);
        // The new node inherits from the old one the position of the parents
        for(uint j=1; j<=key_nr; j++)
        {
            if(parent_node->get(j)->name() == PARENT)
            {
                // Returns only new nodes of the previous level
                tmp_node = GetParentNode(level-1, j);
                ConnectNodes(tmp_node, new_node, j);
            }
        }
    }

    if(key_nr >= level)
        // Next level
        AddNodes(level+1, key_nr);
}


Vertex* StateGraph::GetParentNode(uint level, uint pos)
{
    Vertex* peers = GetCommands(level);
    Vertex* node;
    uint i = 0;
    while((node=peers->get(++i)) != nullptr)
        if(!node->get(pos)->get())
            return node;

    return nullptr;
}


Vertex* StateGraph::GetCommands(uint level)
{
    if(level > Root->size())
        return nullptr;

    // Search from bottom up
    StateNode* tmp_node = Root;
    for(uint i=1; i<=level; i++)
    {
        if(!tmp_node->get(i)->get())
            // Nothing yet on this level - create level container once
            return GetNewPeersList();
        tmp_node = tmp_node->GetChild(i);
    }
    return tmp_node->Vertex::get(COMMANDS);
}


void StateGraph::ConnectNodes(Vertex* p, Vertex* c, uint i)
{
    p->get(i)->attach(c);

    Vertex* v = c->get(i);
    v->name(PARENT);
    v->attach(p);
}

Vertex* StateGraph::GetNewPeersList()
{
    Vertex* ret = new List(COMMANDS);
    ret->Vertex::attach(get(DEVICE, ANY));
    return ret;
}
