#include "input/sdl/eventhandler.h"
#include "base/user.h"
#include "input/sdl/keyboard.h"
#include <unistd.h>


// C callback used in EventHandlerSdl::Start()
Uint32 PumpCallback(Uint32 i, void* a_handler)
{
    reinterpret_cast<EventHandlerSdl*>(a_handler)->Pump();
    return i;
}



EventHandlerSdl::~EventHandlerSdl()
{
    for (Device* d : m_Devices)
        delete d;
}


void EventHandlerSdl::Start()
{
    // Start only once
    if (Timer)
        return;
    // 25 pix per sec
    uint32_t interval = 1000/25;
    Timer = SDL_AddTimer(interval, &PumpCallback, this);
    if (!Timer)
        exit(1);
    // Stop the main execution line
    pause();
}


void EventHandlerSdl::Pump()
{
    // Executed 25x per sec
    if (ExecNotFinished)
        return;
    ExecNotFinished = true;

    SDL_Event event;
    while (SDL_PollEvent(&event))
        for (DeviceSdl* device : m_Devices)
            if (device->Process(event))
                break;

    // Trigger screen refresh.
    m_User->Update();
    ExecNotFinished = false;
}


Device* EventHandlerSdl::GetDevice(Device::Driver a_deviceId)
{
    DeviceSdl* device = nullptr;
    if (a_deviceId == Device::Keyboard)
    {
        // We have only one keyboard spawning multiple hands
        for (DeviceSdl* d : m_Devices)
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
