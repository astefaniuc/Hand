#ifndef HAND_INPUT_DEVICE_H
#define HAND_INPUT_DEVICE_H

#include <vector>
#include <SDL/SDL.h>
#include "base/module.h"
#include "graph/collection.h"


class InputState;

class Device : public virtual Module
{
public:
    Device();
    ~Device();

    // Implements get(VIEW)
    Interface* GetHmi() override;
    // Sets the key map
    bool Init();
    // Functions accessing the layer object (not possible from here)
    bool Press(int key_id);
    bool Release(int key_id);
    bool IsUnused();
    unsigned GetNumberOfKeys();
    // Return the input state machine
    InputState* GetInputState();
    //  User input handling
    void GetUserInput();
    void Press(SDLKey);
    void Release(SDLKey);

protected:
    /// Callbacks
//    HmiItem* Init

    // Returns the Key symbol at specified position
    Note* GetKey(unsigned index);
    void AddKey(int key_id);
    void DeleteKey(unsigned index);
    // Returns the key number
    int GetKeyIndex(int key_id);

    // Members:
    InputState* m_StateMachine = nullptr;
    // Number of controls
    unsigned m_NumberOfKeys = NUMBER_OF_BUTTONS;
    std::vector<int> m_Keys;

    Collection* m_KeysHmi;
    Interface* m_Hmi = nullptr;
};

#endif // HAND_INPUT_DEVICE_H
