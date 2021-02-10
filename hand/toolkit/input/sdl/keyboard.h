#ifndef HAND_INPUT_KEYBOARD_H
#define HAND_INPUT_KEYBOARD_H

#include "input/sdl/device.h"


class Keyboard : public DeviceSdl
{
public:
    Keyboard() : DeviceSdl(Device::Keyboard) {}

    Hmi::Interface* GetHmi() override { return nullptr; }
    /// Keyboards may have multiple Hands per Device.
    void SetUser(Hand* hand) override { m_Hands.push_back(hand); }
    /// Writes the name of the key at position 'k' in 'out'.
    void GetKeyName(int k, Hmi::Note* out) override;

    bool Process(const SDL_Event& event) override;

private:
    std::vector<Hand*> m_Hands;
};

#endif // HAND_INPUT_KEYBOARD_H
