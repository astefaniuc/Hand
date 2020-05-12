#ifndef HAND_BASE_SERVER_H
#define HAND_BASE_SERVER_H

#include <SDL/SDL.h>
#include <string>

class CUser;
class Device;
class Screen;

class HandServer
{
// Singleton
private:
    HandServer();
    ~HandServer();

public:
    static HandServer* GetInstance();
    // Called from main()
    void Present(const std::string& file);
    // Initializes all subsystems:
    CUser* CreateUser();
    // Starts timer (infinite loop)
    void Start();

    // Called from C callback
    void Pump();

private:
    //  User input handling
    void GetUserInput();
    void Press(SDLKey);
    void Release(SDLKey);

    // Members:
    // The one and only server (singleton)
    static HandServer* Instance;
    // TODO: multiple screens e.g. touch screen device
    Screen* _Screen;
    // The queue for system device events
    SDL_Event Event;
    SDL_TimerID Timer = nullptr;
    bool ExecNotFinished = false;

    // Variables controlling the server behavior - "settings"
    bool CreateNewUserOnOrphanKeyPress = true;
    bool DeleteDeviceIfEmpty = true;
};

#endif // HAND_BASE_SERVER_H
