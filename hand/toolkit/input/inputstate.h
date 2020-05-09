#ifndef HAND_INPUT_INPUTSTATE_H
#define HAND_INPUT_INPUTSTATE_H

#include <vector>
#include <string>
#include "input/node.h"
#include "include/defines.h"


class CUser;

class InputState
{
public:
    // Link to the parent object, the main server
    InputState(Device* a_device);
    virtual ~InputState() {}
    // Stores information about pressed keys by moving up the PressedKey ptr
    bool Press(unsigned index);
    // Stores information about released keys by moving down the ReleasedKey ptr
    bool Release(unsigned index);
    // Resets "pressed" and "released" information
    void reset();
    StateNode* GetKey(key_pointer);
    Vertex* GetCommands(unsigned level);
    // Maps a command node to the given list
    bool GetCommand(Vertex* method, unsigned level);

protected:
    // TODO: do we need to store the pointer?
    CUser* m_User;

    StateNode* NullKey;
    StateNode* PressedKey;
    StateNode* ReleasedKey;
};

#endif // HAND_INPUT_INPUTSTATE_H
