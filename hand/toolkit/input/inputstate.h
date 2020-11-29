#ifndef HAND_INPUT_INPUTSTATE_H
#define HAND_INPUT_INPUTSTATE_H

#include "input/node.h"


class InputState
{
public:
    InputState(unsigned numberOfKeys);
    virtual ~InputState() {}

    // Stores information about pressed keys by moving up the PressedKey ptr
    bool Press(unsigned index);
    // Stores information about released keys by moving down the ReleasedKey ptr
    bool Release(unsigned index);

    bool IsClean() { return (PressedKey == NullKey); }

    StateNode::PeersList* GetCommands(unsigned level);

protected:
    // Resets "pressed" and "released" information
    void Reset();

    StateNode* NullKey;
    StateNode* PressedKey;
    StateNode* ReleasedKey;
};

#endif // HAND_INPUT_INPUTSTATE_H
