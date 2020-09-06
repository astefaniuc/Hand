#include "handserver.h"
#include "modulelib.h"
#include "user.h"
#include "input/device.h"
#include "view/layer/layer.h"
#include "view/theme.h"
#include <unistd.h>
#include <iostream>


HandServer::HandServer(const std::string& a_startApp)
    : m_View("Screen", ""), m_UserViews("Users", "Views"), m_Menu("Menu", "Settings")
{
    m_View.SetView(m_UserViews.GetLayer());
    m_View.SetControls(&m_Menu);

    m_ThemeLoader = new ModuleLib();
    m_Menu.Add(new Note(
            "Theme", "Select default visualization theme for all users.",
            "./binaries/lib/themes/default.so", m_ThemeLoader));

    // TODO: load settings
    m_View.GetLayer()->SetTheme(dynamic_cast<Theme*>(m_ThemeLoader->GetObject()));
    m_View.GetLayer()->GetTheme()->InitScreen(m_View.GetLayer());

    m_Input = new EventHandler();

    CUser* user = CreateUser();
    if (!a_startApp.empty())
    {
        m_AppPath = new Note("Start app", "Passed as program argument", a_startApp);
        if (!user->LoadApp(m_AppPath))
            std::cerr << "Error: can't open app '" << a_startApp << "'.";
    }
}


HandServer::~HandServer()
{
    delete m_ThemeLoader;
    delete m_AppPath;
    delete m_Input;
    for (CUser* user : m_Users)
        delete user;
}


CUser* HandServer::CreateUser()
{
    CUser* user = new CUser(m_Input);
    m_Users.push_back(user);
    m_UserViews.Attach(user->GetHmi());
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
    Timer = SDL_AddTimer(interval, &CallServerPump, this);
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

    if (!m_UserViews.Size())
        // Nothing to show, the normal exit
        exit(0);

    // Wait till next cycle before setting the next content
    // because this deletes the calling object
    m_Input->GetUserInput();

    m_View.GetLayer()->Update();
    m_View.GetLayer()->GetTheme()->UpdateScreen();

    ExecNotFinished = false;
}
