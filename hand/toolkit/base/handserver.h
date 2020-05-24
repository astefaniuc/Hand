#ifndef HAND_BASE_SERVER_H
#define HAND_BASE_SERVER_H

#include <SDL/SDL.h>
#include <string>
#include <vector>


class CUser;
class Device;
class Screen;

class HandServer
{
public:
    HandServer();
    ~HandServer();

    // Initializes all subsystems:
    CUser* CreateUser();
    // Starts timer (infinite loop)
    void Start();

    // Called from C callback
    void Pump();

private:
    // TODO: multiple screens e.g. touch screen device
    Screen* m_Screen = nullptr;
    std::vector<Device*> m_Devices;

    // The queue for system device events
    SDL_TimerID Timer = nullptr;
    bool ExecNotFinished = false;

    // Variables controlling the server behavior - "settings"
    bool CreateNewUserOnOrphanKeyPress = true;
    bool DeleteDeviceIfEmpty = true;
};

#endif // HAND_BASE_SERVER_H
