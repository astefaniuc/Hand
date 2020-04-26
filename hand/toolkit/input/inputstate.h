#ifndef HAND_INPUT_INPUTSTATE_H
#define HAND_INPUT_INPUTSTATE_H

#include <vector>
#include <string>
#include "input/node.h"
#include "include/defines.h"


class Device;
class LayerManager;

class InputState
{
public:
    // Link to the parent object, the main server
    InputState(Vertex* device);
    virtual ~InputState() {}
    // Stores information about pressed keys by moving up the PressedKey ptr
    bool Press(uint index);
    // Stores information about released keys by moving down the ReleasedKey ptr
    bool Release(uint index);
    // Resets "pressed" and "released" information
    void reset();
    StateNode* GetKey(key_pointer);
    Vertex* GetCommands(uint level);
    // Maps a command node to the given list
    bool GetCommand(Vertex* method, uint level);

protected:
    LayerManager* LM;

    StateNode* NullKey;
    StateNode* PressedKey;
    StateNode* ReleasedKey;
};

#endif // HAND_INPUT_INPUTSTATE_H
