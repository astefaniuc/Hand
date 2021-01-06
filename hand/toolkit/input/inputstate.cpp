#include "input/inputstate.h"


InputState::InputState(unsigned a_numberOfKeys) : m_NumberOfKeys(a_numberOfKeys)
{
    NullKey = PressedKey = ReleasedKey = new StateNode(m_NumberOfKeys, new StateNode::PeersList());
    for (unsigned i = 0; i < m_NumberOfKeys; ++i)
        AddNodes(1, i);
}


InputState::~InputState()
{
    int i = m_NumberOfKeys;
    while (i >= 0)
    {
        StateNode::PeersList* nodes = GetPeersList(i);
        for (size_t j = 0; j < nodes->size(); ++j)
            delete nodes->at(j);

        delete nodes;
        --i;
    }
}


bool InputState::Press(unsigned k)
{

    PressedKey = ReleasedKey = ReleasedKey->GetChild(k);
    if (!PressedKey)
    {
        Reset();
        return false;
    }

    return true;
}


bool InputState::Release(unsigned k)
{
    // Move the ReleasedKey down to the current key combination node
    if ((ReleasedKey = ReleasedKey->GetParent(k)) == nullptr)
    {
        // Some events got lost, this command is corrupted
        Reset();
        return false;
    }

    if (ReleasedKey == NullKey)
        // All pressed keys have been released
        Reset();

    return true;
}


void InputState::Reset()
{
    PressedKey = ReleasedKey = NullKey;
}


StateNode::PeersList* InputState::GetCommands(unsigned level)
{
    // Search from bottom up
    StateNode* node = NullKey;
    for (unsigned i = 0; i < level; ++i)
        node = node->GetFirstChild();

    return node->GetPeersList();
}


void InputState::AddNodes(unsigned level, unsigned pos)
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


StateNode* InputState::GetParentNode(unsigned level, unsigned pos)
{
    StateNode::PeersList* peers = GetPeersList(level);
    for (StateNode* node : *peers)
        if (!node->GetParent(pos) && !node->GetChild(pos))
            return node;

    return nullptr;
}


StateNode::PeersList* InputState::GetPeersList(unsigned level)
{
    StateNode* node = NullKey;
    for (unsigned i = 0; i < level; ++i)
        node = node->GetFirstChild();

    if (node)
        return node->GetPeersList();

    return nullptr;
}


void InputState::ConnectNodes(StateNode* parent, StateNode* child, unsigned position)
{
    parent->SetChild(child, position);
    child->SetParent(parent, position);
}
