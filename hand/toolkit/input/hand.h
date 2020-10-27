#ifndef HAND_INPUT_HAND_H
#define HAND_INPUT_HAND_H

#include <vector>
#include "base/module.h"
#include "data/collection.h"


enum default_number_of_items
{
    NUMBER_OF_BUTTONS = 5,
    MAX_NUMBER_OF_BUTTONS = 10
};


enum key_position
{
    THUMB,
    FOREFINGER,
    MIDDLEFINGER,
    RINGFINGER,
    LITTLEFINGER
};


class Device;
class InputState;
class Interface;

class Hand : public Module
{
public:
    Hand(Device* dev);
    ~Hand();

    HmiItem* GetHmi() override;
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

    Interface* m_InitScreen = nullptr;
};

#endif //HAND_INPUT_HAND_H
