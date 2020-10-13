#include "input/statetree.h"


StateNode* StateGraph::Create(unsigned numberOfKeys)
{
    m_NumberOfKeys = numberOfKeys;
    // Create the root/null key
    m_Root = new StateNode(m_NumberOfKeys, new StateNode::PeersList());
    for (unsigned i = 0; i < m_NumberOfKeys; ++i)
        AddNodes(1, i);

    return m_Root;
}


void StateGraph::AddNodes(unsigned level, unsigned pos)
{
    StateNode::PeersList* parentNodes = GetPeersList(level - 1);
    StateNode::PeersList* currentNodes = GetPeersList(level);
    if (!currentNodes)
        currentNodes = new StateNode::PeersList();

    // Create a new node for each one existing one level down
    for (unsigned i = 0; i < parentNodes->size(); ++i)
    {
        StateNode* parent = (*parentNodes)[i];
        // Ignore nodes inserted for the current key
        if (parent->GetParent(pos))
            continue;

        StateNode* child = new StateNode(m_NumberOfKeys, currentNodes);
        ConnectNodes(parent, child, pos);
        // The new node inherits from the old one the position of the parents
        for (unsigned j = 0; j < pos; ++j)
            if (parent->GetParent(j))
                ConnectNodes(GetParentNode(level - 1, j), child, j);
    }

    if (pos >= level)
        // Next level
        AddNodes(level + 1, pos);
}


// Returns only new nodes of the previous level
StateNode* StateGraph::GetParentNode(unsigned level, unsigned pos)
{
    StateNode::PeersList* peers = GetPeersList(level);
    for (StateNode* node : *peers)
        if (!node->GetParent(pos) && !node->GetChild(pos))
            return node;

    return nullptr;
}


StateNode::PeersList* StateGraph::GetPeersList(unsigned level)
{
    StateNode* node = m_Root;
    for (unsigned i = 0; i < level; ++i)
        node = node->GetFirstChild();

    if (node)
        return node->GetPeersList();

    return nullptr;
}


void StateGraph::ConnectNodes(StateNode* parent, StateNode* child, unsigned position)
{
    parent->SetChild(child, position);
    child->SetParent(parent, position);
}
