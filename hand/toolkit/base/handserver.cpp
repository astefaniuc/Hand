#include "handserver.h"
#include "user.h"
#include "input/device.h"
#include "input/keyboard.h"
#include <unistd.h>


// C callback used in HandServer::Start()
Uint32 CallServerPump(Uint32 i, void* a_handler)
{
    reinterpret_cast<EventHandlerSDL*>(a_handler)->Pump();
    return i;
}



EventHandlerSDL::~EventHandlerSDL()
{
    for (Device* d : m_Devices)
        delete d;
}


void EventHandlerSDL::Start()
{
    // Start only once
    if (Timer)
        return;
    // 25 pix per sec
    uint32_t interval = 1000/25;
    Timer = SDL_AddTimer(interval, &CallServerPump, this);
    if (!Timer)
        exit(1);
    // Stop the main execution line
    pause();
}


void EventHandlerSDL::Pump()
{
    // Executed 25x per sec
    if (ExecNotFinished)
        return;
    ExecNotFinished = true;

    SDL_Event event;
    while (SDL_PollEvent(&event))
        for (Device* device : m_Devices)
            if (device->Process(event))
                break;

    m_User->Update();
    ExecNotFinished = false;
}


Device* EventHandlerSDL::GetDevice(Device::Driver a_deviceId)
{
    Device* device = nullptr;
    if (a_deviceId == Device::Keyboard)
    {
        // We have only one keyboard spawning multiple hands
        for (Device* d : m_Devices)
        {
            if (d->GetTypeId() == Device::Keyboard)
            {
                device = d;
                break;
            }
        }
    }
    // TODO else, other devices
    if (!device)
    {
        device = new Keyboard();
        m_Devices.push_back(device);
    }
    return device;
}
