#include "input/device.h"
#include "input/hand.h"
#include "input/keyboard.h"


const std::string DriverNames[] =
{
    "Keyboard",
    "Mouse",
    "Controller"
};


Device::Driver Device::GetDriverId(const std::string& a_typeName)
{
    for (unsigned i = 0; i < 3; ++i)
        if (DriverNames[i] == a_typeName)
            return (Device::Driver)i;
    return Keyboard;
}

const std::string& Device::GetDriverName(Device::Driver id)
{
    return DriverNames[id];
}


EventHandler::~EventHandler()
{
    for (Device* d : m_Devices)
        delete d;
}


Hand* EventHandler::CreateHand(Device::Driver a_deviceId)
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
    return new Hand(device);
}


void EventHandler::GetUserInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
        for (Device* device : m_Devices)
            if (device->Process(event))
                break;
}
