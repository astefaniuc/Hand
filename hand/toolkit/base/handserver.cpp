#include "base/handserver.h"
#include <unistd.h>
#include "view/layer/user.h"
#include "input/device.h"
#include "view/screen.h"
#include "graph/data.h"
#include "base/filesystem.h"


HandServer::HandServer()
{
    m_Screen = new Screen();
}


HandServer::~HandServer()
{
    delete m_Screen;
}


CUser* HandServer::CreateUser()
{
    CUser* user = new CUser();

    Device* device = new Device();
    m_Devices.push_back(device);
    // Create device object with input state
    user->SetDevice(device);
    user->SetBuffer(m_Screen->GetSurface(user));

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
    for (Device* device : m_Devices)
        device->GetUserInput();

    if (!m_Screen->ShowSurface())
        // Nothing to show, the normal exit
        exit(0);

    ExecNotFinished = false;
}
