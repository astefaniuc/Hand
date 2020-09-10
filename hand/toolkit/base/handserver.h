#ifndef HAND_BASE_SERVER_H
#define HAND_BASE_SERVER_H

#include "input/device.h"
#include <SDL/SDL.h>
#include <vector>


class CUser;

class EventHandler
{
public:
    virtual ~EventHandler() {}

    // Store the event consumer.
    virtual void SetUser(CUser*) = 0;
    virtual Device* GetDevice(Device::Driver device) = 0;

    virtual void Start() = 0;
};


class EventHandlerSDL : public EventHandler
{
public:
    ~EventHandlerSDL();

    void SetUser(CUser* consumer) override { m_User = consumer; }
    /// Creates a new device.
    Device* GetDevice(Device::Driver device) override;
    /// Starts timer (infinite loop)
    void Start() override;

    /// Called from C callback
    void Pump();

private:
    CUser* m_User = nullptr;
    std::vector<Device*> m_Devices;

    // The queue for system device events
    SDL_TimerID Timer = nullptr;
    bool ExecNotFinished = false;
};

#endif // HAND_BASE_SERVER_H
