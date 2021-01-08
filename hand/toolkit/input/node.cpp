#include "input/node.h"


StateNode::StateNode(unsigned size, PeersList* peers)
    : m_Links(size),
      m_Peers(peers)
{
    // Adds itself to the peers list
    m_Peers->push_back(this);
}


void StateNode::SetParent(StateNode* parent, unsigned pos)
{
    Link& ret = m_Links[pos];
    ret.IsParent = true;
    ret.Item = parent;
}


StateNode* StateNode::GetParent(unsigned pos)
{
    const Link& ret = m_Links[pos];
    if (ret.IsParent)
        return ret.Item;

    return nullptr;
}


void StateNode::SetChild(StateNode* child, unsigned pos)
{
    Link& ret = m_Links[pos];
    ret.IsParent = false;
    ret.Item = child;
}


StateNode* StateNode::GetChild(unsigned pos)
{
    const Link& ret = m_Links[pos];
    if (!ret.IsParent)
        return ret.Item;

    return nullptr;
}


StateNode* StateNode::GetFirstChild() const
{
    for (const Link& ret : m_Links)
        if (!ret.IsParent && ret.Item)
            return ret.Item;
    // Never happens
    return nullptr;
}


bool StateNode::Assign(Layer* layer)
{
    if (m_Layer)
        return false;

    m_Layer = layer;
    return true;
}


bool StateNode::Clear(Layer* layer)
{
    if (m_Layer != layer)
        return false;

    m_Layer = nullptr;
    return true;
}
