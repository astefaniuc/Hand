#ifndef HAND_BASE_EVENTHANDLER_H
#define HAND_BASE_EVENTHANDLER_H

#include "input/device.h"


class User;

class EventHandler
{
public:
    virtual ~EventHandler() {}

    // Store the event consumer.
    virtual void SetUser(User*) = 0;
    virtual Device* GetDevice(Device::Driver device) = 0;

    virtual void Start() = 0;
    virtual void Stop() = 0;
};

#endif // HAND_BASE_EVENTHANDLER_H
