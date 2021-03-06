#include "input/sdl/eventhandler.h"
#include "base/user.h"
#include "input/sdl/keyboard.h"


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
    if (m_Timer)
        return;
    // 25 pix per sec
    uint32_t interval = 1000/60;
    m_Timer = SDL_AddTimer(interval, &PumpCallback, this);
    if (!m_Timer)
        exit(1);
}


void EventHandlerSdl::Stop()
{
    if (!m_Timer)
        return;
    SDL_RemoveTimer(m_Timer);
}


void EventHandlerSdl::Pump()
{
    std::unique_lock<std::mutex> lock(m_Execution, std::defer_lock);
    if (!lock.try_lock())
        return;

    bool forceRedraw = false;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                m_User->Stop();
                return;
            }
            forceRedraw = true;
        }
        else
            for (DeviceSdl* device : m_Devices)
                if (device->Process(event))
                    break;
    }

    // Trigger screen refresh.
    m_User->Update(forceRedraw);
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
