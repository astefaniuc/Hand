#ifndef HAND_INPUT_HAND_H
#define HAND_INPUT_HAND_H

#include "base/module.h"
#include "data/vector.h"
#include "input/chord.h"
#include <map>
#include <vector>


enum default_number_of_items
{
    NUMBER_OF_BUTTONS = 5,
    MAX_NUMBER_OF_BUTTONS = 10
};



class Device;
class InputState;
namespace Hmi { class Interface; }

class Hand : public Module
{
public:
    Hand(Device* dev);
    ~Hand();

    Hmi::Item* GetHmi() override { return nullptr; }
    Hmi::Item* GetInitScreen();
    // Sets the key map
    bool Init();

    bool Press(int keyId);
    bool Release(int keyId);

    unsigned GetNumberOfKeys() { return m_NumberOfKeys; }

    bool SetFocus(Layer* view);

    /// Finger names.
    static const std::string Finger[5];

private:
    // Returns the key number
    int GetKeyIndex(int keyId);

    // Returns the Key symbol at specified position
    Hmi::Note* GetKey(unsigned index);
    void AddKey(int key_id);
    void DeleteKey(unsigned index);

    void BindChords(Layer* focus);

    // Number of controls
    unsigned m_NumberOfKeys = NUMBER_OF_BUTTONS;
    std::vector<int> m_Keys;
    Hmi::Vector* m_KeysHmi;
    Device* m_Device;
    InputState* m_InputState = nullptr;

    Chord m_Record;
    std::map<Hmi::Item*, Chord> m_Commands;

    Hmi::Interface* m_InitScreen = nullptr;
};

#endif //HAND_INPUT_HAND_H
