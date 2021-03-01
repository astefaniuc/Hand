#ifndef HAND_INPUT_HAND_H
#define HAND_INPUT_HAND_H

#include "base/module.h"
#include "data/vector.h"
#include "input/chord.h"
#include "input/interaction.h"
#include <vector>


enum default_number_of_items
{
    NUMBER_OF_BUTTONS = 5,
    MAX_NUMBER_OF_BUTTONS = 10
};



class Device;
class InputState;
namespace Interaction { class Control; }

class Hand : public Module
{
public:
    enum InteractionLevel
    {
        Focus,
        Peripherial
    };

    Hand(Device* dev);
    ~Hand();

    // Sets the key map
    bool Init();
    void SetInteraction(Interaction::Control* control) { m_Interaction = control; }

    Hmi::Interface* GetHmi() override { return nullptr; }
    Layer* GetInitScreen();

    bool Press(int keyId);
    bool Release(int keyId);

    unsigned GetNumberOfKeys() { return m_NumberOfKeys; }

    Chord* Assign(Layer* item, InteractionLevel interaction);
    Chord* Reserve(Chord* shortcut);

    // Returns the Key symbol at specified position
    Hmi::Note* GetKey(unsigned index);

    /// Finger names.
    static const std::string Finger[5];

private:
    // Returns the key number
    int GetKeyIndex(int keyId);

    void AddKey(int key_id);
    void DeleteKey(unsigned index);

    void InitInitLayer(Layer* l);
    void InitHandLayer(Layer* l);

    // Number of fingers/hw controls
    unsigned m_NumberOfKeys = NUMBER_OF_BUTTONS;
    unsigned m_PeripherialLevel = 3;

    std::vector<int> m_Keys;
    Hmi::Vector* m_KeysHmi;
    Device* m_Device;
    InputState* m_InputState = nullptr;

    Chord m_Record;
    Interaction::Control* m_Interaction;
};

#endif //HAND_INPUT_HAND_H
