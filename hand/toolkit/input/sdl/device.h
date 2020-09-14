#ifndef HAND_INPUT_SDL_DEVICE_H
#define HAND_INPUT_SDL_DEVICE_H

#include "input/device.h"
#include <SDL/SDL.h>


class DeviceSdl : public Device
{
public:
    DeviceSdl(Driver id) : Device(id) {}

    virtual bool Process(const SDL_Event& event) = 0;
};

#endif // HAND_INPUT_SDL_DEVICE_H
