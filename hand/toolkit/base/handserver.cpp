#include "base/handserver.h"
#include <unistd.h>
#include "view/layer/user.h"
#include "input/device.h"
#include "view/screen.h"
#include "base/handapploader.h"
#include <iostream>


HandServer::HandServer(const std::string& a_startApp)
{
    m_Screen = new Screen();
    m_Input = new EventHandler();

    CUser* user = CreateUser();
    if (!a_startApp.empty())
    {
        m_AppPath = new Note("Start app", "Passed as program argument", a_startApp);
        ModuleLib* app = new ModuleLib();
        app->SetItem(m_AppPath);
        if (app->Load())
        {
            m_RunningApps.push_back(app);
            user->SetContent(app->GetHmi());
        }
        else
        {
            std::cerr << "Error: can't open app '" << a_startApp << "'.";
            delete app;
        }
    }
}


HandServer::~HandServer()
{
    for (ModuleLib* app : m_RunningApps)
        delete app;
    delete m_AppPath;
    delete m_Input;
    delete m_Screen;
}


CUser* HandServer::CreateUser()
{
    CUser* user = new CUser(m_Input, m_Screen);

    return user;
}

// C Method used in HandServer::Start()
// C++ methods can't be called directly as a callback
Uint32 CallServerPump(Uint32 i, void* server)
{
    HandServer* hand = reinterpret_cast<HandServer*>(server);
    hand->Pump();
    return i;
}


void HandServer::Start()
{
    // Start only once
    if (Timer)
        return;
    // 25 pix per sec
    Uint32 interval = 1000/25;
    Timer = SDL_AddTimer(interval, &CallServerPump, (void*)(this));
    if (!Timer)
        exit(1);
    // Stop the main execution line
    pause();
}


void HandServer::Pump()
{
    // Executed 25x per sec
    if (ExecNotFinished)
        return;
    ExecNotFinished = true;
    // Wait till next cycle before setting the next content
    // because this deletes the calling object
    m_Input->GetUserInput();
    if (!m_Screen->ShowSurface())
        // Nothing to show, the normal exit
        exit(0);

    ExecNotFinished = false;
}
