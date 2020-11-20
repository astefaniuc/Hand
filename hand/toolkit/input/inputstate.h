#ifndef HAND_INPUT_INPUTSTATE_H
#define HAND_INPUT_INPUTSTATE_H

#include "input/node.h"
#include "input/chord.h"


class InputState
{
public:
    InputState(unsigned numberOfKeys);
    virtual ~InputState() {}
    // Stores information about pressed keys by moving up the PressedKey ptr
    bool Press(unsigned index);
    // Stores information about released keys by moving down the ReleasedKey ptr
    bool Release(unsigned index);
    // Resets "pressed" and "released" information
    void Reset();

    bool Bind(HmiItem* method, const Chord& chord);
    StateNode::PeersList* GetCommands(unsigned level);
    // Maps a command node to the given list
    bool GetCommand(HmiItem* method, unsigned level);

protected:
    StateNode* NullKey;
    StateNode* PressedKey;
    StateNode* ReleasedKey;
};

#endif // HAND_INPUT_INPUTSTATE_H
