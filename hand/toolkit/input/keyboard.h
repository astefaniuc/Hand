#ifndef HAND_INPUT_KEYBOARD_H
#define HAND_INPUT_KEYBOARD_H

#include "input/device.h"


class Keyboard : public Device
{
public:
    Keyboard() : Device(Device::Keyboard) {}

    /// Keyboards may have multiple Hands per Device.
    void SetUser(Hand* hand) override { m_Hands.push_back(hand); }
    /// Writes the name of the key at position 'k' in 'out'.
    void GetKeyName(int k, Note* out) override;

    bool Process(const SDL_Event& event) override;

private:
    std::vector<Hand*> m_Hands;
};

#endif // HAND_INPUT_KEYBOARD_H
