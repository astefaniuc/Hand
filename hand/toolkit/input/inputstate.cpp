#include "input/inputstate.h"
#include "input/statetree.h"
#include "graph/hmiitem.h"


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
    // Move the ReleasedKey up the the three
    // and the ReleasedKey down to the current key combination node
    if( (PressedKey == NullKey) ||
        ((ReleasedKey = ReleasedKey->GetParent(k)) == nullptr))
    {
        // Some events get lost, this command is corrupted
        Reset();
        return false;
    }

    // Execute command when all pressed keys have been released
    if (ReleasedKey == NullKey)
    {
        // Inform the GUI
        HmiItem* action = PressedKey->GetAction();
        if (action)
            action->Activate();

        Reset();
    }
    return true;
}


void InputState::Reset(void)
{
    PressedKey = ReleasedKey = NullKey;
}


StateNode* InputState::GetKey(key_pointer key)
{
    switch(key)
    {
    case ROOT:
        return NullKey;
    case PRESSED:
        return PressedKey;
    case RELEASED:
        return ReleasedKey;
    default:
        return nullptr;
    }
}


bool InputState::GetCommand(HmiItem* target, unsigned level)
{
    if (!NullKey)
        // Not initialized
        return false;

    // Get the list of available commands
    StateNode::PeersList* levelCmds = GetCommands(level);
    for (StateNode* node : *levelCmds)
    {
        // Bind the functional list to the state graph (command) node.
        if (!node->GetAction())
        {
            node->SetAction(target);
            return true;
        }
    }
    return false;
}


StateNode::PeersList* InputState::GetCommands(unsigned level)
{
    // Search from bottom up
    StateNode* node = NullKey;
    for (unsigned i = 0; i < level; ++i)
        node = node->GetFirstChild();

    return node->GetPeersList();
}
