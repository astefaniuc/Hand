#include "base/handserver.h"

#include "../view/layer/user.h"
#include "input/device.h"
#include "view/screen.h"
#include "graph/data.h"
#include "base/filesystem.h"


HandServer::HandServer()
{
    _Screen = new Screen();
}


HandServer::~HandServer()
{
    delete _Screen;
}


HandServer* HandServer::Instance = nullptr;

HandServer* HandServer::GetInstance()
{
    if(!Instance)
    {
        Instance = new HandServer();
        Instance->Init();
    }
    return Instance;
}


void HandServer::Present(const std::string& file)
{
    Vertex* app = new Path(file);
    while(Resolve(app))
    {
        Vertex* ot = app->Vertex::get(REQUEST)->get();
        if(!ot)
            break;
        app = app->Vertex::get(ot->name(), ANY);
    }
    CreateUser()->Layer::SetContent(app);
}


CUser* HandServer::CreateUser()
{
    CUser* user = new CUser();
    _Screen->SetUser(user);

    // Create device object with input state
    user->SetDevice(new Device());
    user->SetBuffer(_Screen->GetSurface());

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


// C Method used in HandServer::Start()
// Removes all devices but the keyboard from the event queue
int EventFilter(const SDL_Event *event)
{
    if((event->type == SDL_KEYDOWN) || (event->type == SDL_KEYUP))
        return 1;
    return 0;
}


void HandServer::Start()
{
    // Start only once
    if(Timer)
        return;
    // Only for speed
    SDL_SetEventFilter(EventFilter);
    // 25 pix per sec
    Uint32 interval = 1000/25;
    Timer = SDL_AddTimer(interval, &CallServerPump, (void*)(this));
    if(!Timer)
        exit(1);
    // Stop the main execution line
    pause();
}


void HandServer::Pump()
{
    // Executed 25x per sec
    if(ExecNotFinished)
        return;
    ExecNotFinished = true;
    // Wait till next cycle before setting the next content
    // because this deletes the calling object
    GetUserInput();
    if(!_Screen->ShowSurface())
        // Nothing to show, the normal exit
        exit(0);

    ExecNotFinished = false;
}


void HandServer::GetUserInput()
{
    while(SDL_PollEvent(&Event))
    {
        switch(Event.type)
        {
            case SDL_KEYDOWN:
                Press(Event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                Release(Event.key.keysym.sym);
                break;
        }
    }
}


void HandServer::Press(SDLKey k)
{
    Vertex* all_dev = get("Devices");
    Device* dev;
    unsigned i = 0;
    while((dev=dynamic_cast<Device*>(all_dev->get(++i))) != nullptr)
        if(dev->Press(k))
            return;

    if(CreateNewUserOnOrphanKeyPress)
    {
        // Create a new device if the pressed key doesn't fits to an existing one
        GetUser();
        Press(k);
    }
}


void HandServer::Release(SDLKey k)
{
    CUser* lm = _Screen->GetUser();
    // Gets the device
    Vertex* all_dev = get("Devices");
    Device* dev;
    unsigned i = 0;
    while((dev=dynamic_cast<Device*>(all_dev->get(++i))) != nullptr)
    {
        if(dev->Release(k))
        {
            if(DeleteDeviceIfEmpty
               && dev->IsUnused()
               && (lm->size() > 1))
            {
                // Exit last layer
                delete lm->get(lm->size());
            }
            return;
        }
    }
}
