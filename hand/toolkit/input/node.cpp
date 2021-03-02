#include "input/node.h"
#include "input/chord.h"


StateNode::StateNode(unsigned size, PeersList* peers)
    : m_Links(size),
      m_Peers(peers)
{
    m_Peers->push_back(this);
}


StateNode::~StateNode()
{
    delete m_Chord;
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


Chord* StateNode::GetChord()
{
    if (!m_Chord)
    {
        m_Chord = new Chord();
        for (unsigned i = 0; i < m_Links.size(); ++i)
            if (GetParent(i))
                m_Chord->Keys.push_back(Chord::Finger(i));
    }
    return m_Chord;
}
