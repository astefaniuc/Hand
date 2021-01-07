#ifndef HAND_INPUT_INPUTSTATE_H
#define HAND_INPUT_INPUTSTATE_H

#include "input/node.h"


class InputState
{
public:
    InputState(unsigned numberOfKeys);
    ~InputState();

    // Stores information about pressed keys by moving up the PressedKey ptr
    bool Press(unsigned index);
    // Stores information about released keys by moving down the ReleasedKey ptr
    bool Release(unsigned index);

    bool IsClean() { return (PressedKey == NullKey); }

    StateNode::PeersList* GetCommands(unsigned level);

private:
    void Reset() { PressedKey = ReleasedKey = NullKey; }

    // Graph creation

    void AddNodes(unsigned level, unsigned keyNumber);
    void ConnectNodes(StateNode* parent, StateNode* child, unsigned pos);
    // Returns only new nodes of the previous level
    StateNode* GetParentNode(unsigned level, unsigned pos);
    StateNode::PeersList* GetPeersList(unsigned level);

    unsigned m_NumberOfKeys = 0;

    StateNode* NullKey;
    StateNode* PressedKey;
    StateNode* ReleasedKey;
};

#endif // HAND_INPUT_INPUTSTATE_H
