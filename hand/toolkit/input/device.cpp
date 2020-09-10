#include "input/device.h"


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
