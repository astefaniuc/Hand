#include "input/statetree.h"


StateNode* StateGraph::Create(unsigned a_numberOfKeys)
{
    // Create the root/null key
    m_Root = new StateNode(a_numberOfKeys, new StateNode::PeersList());
    // Build the key tree on top
    for(unsigned i = 1; i <= a_numberOfKeys; ++i)
        // Build the key tree bellow
        AddNodes(1, i);

    return m_Root;
}


void StateGraph::AddNodes(unsigned level, unsigned key_nr)
{
    // Previous level nodes
    StateNode::PeersList* parentNodes = GetPeersList(level-1);
    // Current level nodes
    StateNode::PeersList* currentNodes = new StateNode::PeersList();

    // Create a new node for each one existing one level down
    for (unsigned i = 0; i < parentNodes->size(); ++i)
    {
        StateNode* parent = (*parentNodes)[i];
        // Ignore nodes inserted for the current key
        if(parent->GetChild(key_nr))
            continue;

        // Appends also the new node to c_nodes
        StateNode* new_node = new StateNode(GetPeersList(0)->size(), currentNodes);
        // Connect new and old node at key number position
        ConnectNodes(parent, new_node, key_nr);
        // The new node inherits from the old one the position of the parents
        for (unsigned j = 1; j < key_nr; ++j)
            if (parent->GetParent(j))
                ConnectNodes(GetParentNode(level-1, j), new_node, j);
    }

    if (key_nr > level)
        // Next level
        AddNodes(level+1, key_nr);
}


// Returns only new nodes of the previous level
StateNode* StateGraph::GetParentNode(unsigned level, unsigned pos)
{
    StateNode::PeersList* peers = GetPeersList(level);
    for (StateNode* node : *peers)
        if(!node->GetParent(pos))
            return node;

    return nullptr;
}


StateNode::PeersList* StateGraph::GetPeersList(unsigned level)
{
    StateNode* node = m_Root;
    for (unsigned i = 0; i < level; ++i)
        node = node->GetChild(i);

    return node->GetPeersList();
}


void StateGraph::ConnectNodes(StateNode* a_parent, StateNode* a_child, unsigned a_position)
{
    a_parent->SetChild(a_child, a_position);
    a_child->SetParent(a_parent, a_position);
}
