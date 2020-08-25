#ifndef HAND_BASE_SERVER_H
#define HAND_BASE_SERVER_H

#include "graph/data.h"
#include "graph/interface.h"
#include "graph/collection.h"
#include <SDL/SDL.h>
#include <string>
#include <vector>


class CUser;
class EventHandler;
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
    EventHandler* m_Input;
    ModuleLib* m_ThemeLoader;

    std::vector<CUser*> m_Users;
    Note* m_AppPath = nullptr;
    // This is the entry point for the whole visualization tree.
    Interface m_View;
    Collection m_UserViews;
    Collection m_Menu;

    // The queue for system device events
    SDL_TimerID Timer = nullptr;
    bool ExecNotFinished = false;
};

#endif // HAND_BASE_SERVER_H
