#ifndef HAND_INPUT_DEVICE_H
#define HAND_INPUT_DEVICE_H

#include <vector>
#include "base/handapp.h"
#include "graph/data.h"


class InputState;

class Device : public virtual HandApp
{
public:
    Device();
    ~Device();

    using HandApp::get;
    // Implements get(VIEW)
    Vertex* get(const std::string& name) override;
    // Sets the key map
    bool Init();
    // Functions accessing the layer object (not possible from here)
    bool Press(int key_id);
    bool Release(int key_id);
    bool IsUnused();
    uint GetNumberOfKeys();
    // Return the input state machine
    InputState* GetInputState();

protected:
    // Returns the Key symbol at specified position
    Note* GetKey(uint index);
    void AddKey(int key_id);
    void DeleteKey(uint index);
    // Returns the key number
    int GetKeyIndex(int key_id);
    // Layout factory for KEYLIST
    bool GetKeyListLayout(Vertex* layout);

    // Members:
    InputState* StateMachine = nullptr;
    // Number of controls
    uint numberOfKeys = NUMBER_OF_BUTTONS;
    std::vector<int> Keys;
    std::vector<int>::iterator currentKey;
};

#endif // HAND_INPUT_DEVICE_H
