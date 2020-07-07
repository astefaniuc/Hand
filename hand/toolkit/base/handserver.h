#ifndef HAND_BASE_SERVER_H
#define HAND_BASE_SERVER_H

#include <SDL/SDL.h>
#include <string>
#include <vector>
#include "graph/data.h"


class CUser;
class EventHandler;
class Screen;
class ModuleLib;

class HandServer
{
public:
    HandServer(const std::string& startApp);
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
    EventHandler* m_Input;
    std::vector<ModuleLib*> m_RunningApps;
    Note* m_AppPath = nullptr;

    // The queue for system device events
    SDL_TimerID Timer = nullptr;
    bool ExecNotFinished = false;
};

#endif // HAND_BASE_SERVER_H
