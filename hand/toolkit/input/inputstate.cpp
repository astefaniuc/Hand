#include "input/inputstate.h"
#include "input/statetree.h"


InputState::InputState(unsigned a_numberOfKeys)
{
    StateGraph state;
    NullKey = PressedKey = ReleasedKey = state.Create(a_numberOfKeys);
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
