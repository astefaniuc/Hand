#ifndef HAND_BASE_SDL_EVENTHANDLER_H
#define HAND_BASE_SDL_EVENTHANDLER_H

#include "input/eventhandler.h"
#include <SDL2/SDL.h>
#include <mutex>
#include <vector>


class DeviceSdl;

class EventHandlerSdl : public EventHandler
{
public:
    ~EventHandlerSdl();

    void SetUser(User* consumer) override { m_User = consumer; }
    /// Creates a new device.
    Device* GetDevice(Device::Driver device) override;
    /// Starts timer (infinite loop)
    void Start() override;
    void Stop() override;

    /// Called from C callback
    void Pump();

private:
    User* m_User = nullptr;
    std::vector<DeviceSdl*> m_Devices;

    // The queue for system device events
    SDL_TimerID m_Timer = 0;
    std::mutex m_Execution;
};

#endif // HAND_BASE_SDL_EVENTHANDLER_H
