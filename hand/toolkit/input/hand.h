#ifndef HAND_INPUT_HAND_H
#define HAND_INPUT_HAND_H

#include <vector>
#include "base/module.h"
#include "graph/collection.h"


class Device;
class InputState;

class Hand : public Module
{
public:
    Hand(Device* dev);
    ~Hand();

    Interface* GetHmi() override;
    // Sets the key map
    bool Init();

    bool Press(int keyId);
    bool Release(int keyId);
    // Return the input state machine
    InputState* GetInputState() { return m_StateMachine; }
    unsigned GetNumberOfKeys() { return m_NumberOfKeys; }

private:
    // Returns the key number
    int GetKeyIndex(int keyId);

    // Returns the Key symbol at specified position
    Note* GetKey(unsigned index);
    void AddKey(int key_id);
    void DeleteKey(unsigned index);

    // Number of controls
    unsigned m_NumberOfKeys = NUMBER_OF_BUTTONS;
    std::vector<int> m_Keys;
    Collection* m_KeysHmi;
    Device* m_Device;
    InputState* m_StateMachine = nullptr;
    Interface* m_Hmi = nullptr;
};

#endif //HAND_INPUT_HAND_H
