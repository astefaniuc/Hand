/*
 *  Copyright 2012 Alex Stefaniuc
 *
 *  This file is part of Hand.
 *
 *  Hand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  Hand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with Hand. If not, see <http://www.gnu.org/licenses/>.
 */

#include "base/handserver.h"
#include "view/layer/layer.h"
#include "view/layer/layermanager.h"
#include "input/device.h"
#include "input/inputstate.h"
#include "view/screen.h"


using namespace std;


HandServer* HandServer::Instance = NULL;


HandServer* HandServer::GetInstance()
{
    if(Instance == NULL)
    {
        // Step 1 (of 3):
        Instance = new HandServer();
        // Step 2 (of 3): this calls subsequently GetInstance() thus have to be called
        // after initializing Instance
        Instance->Start();
    }
    return Instance;
}


HandServer::HandServer()
{
    ExecNotFinished = false;

    // DrawObject
    CreateNewUserOnOrphanKeyPress = true;
    DeleteDeviceIfEmpty = true;
    Timer = NULL;
}


HandServer::~HandServer()
{
    vector<LayerManager*>::iterator curr = LayerMgrs.begin();
    LayerManager* tmp;
    // Recursively deletes sub-layers
    while(curr!=LayerMgrs.end())
    {
        tmp = (*curr);
        curr = LayerMgrs.erase(curr);
        delete tmp;
    }
    delete _Screen;
}


void HandServer::Start()
{
    _Screen = new Screen();
    LayerManager* layer_mgr = GetLayerManager();
    _Screen->SetLayerManager(layer_mgr);
}


void HandServer::Exit(Layer* lm)
{
    if(LayerMgrs.size() <= 1)
        exit(0);

    // Multiple LayerManager => don't exit; UnregisterDevice()
    vector<LayerManager*>::iterator curr;
    LayerManager* tmp;
    // Recursively deletes sub-layers
    for(curr=LayerMgrs.begin(); curr!=LayerMgrs.end(); curr++)
    {
        tmp = (*curr);
        if(tmp == lm)
        {
            Unregister(tmp->GetDevice());
            LayerMgrs.erase(curr);
            delete tmp;
            break;
        }
    }
    SetLayerManagerPositions();
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
    LayerManager* layer_mgr;
    vector<LayerManager*>::iterator curr;
    vector<LayerManager*>::iterator end = LayerMgrs.end();
    for(curr=LayerMgrs.begin(); curr!=end; curr++)
    {
        layer_mgr = (*curr);
        layer_mgr->Update(false);
    }
    _Screen->ShowSurface();
    ExecNotFinished = false;
}


LayerManager* HandServer::GetLayerManager()
{
    // Start the layer manager
    LayerManager* layer_mgr = new LayerManager();
    LayerMgrs.push_back(layer_mgr);
    layer_mgr->SetOwner(this);
//    layer_mgr->Init();

    // Create device object with input state
    Device* device = new Device();
    this->Register(device);
    layer_mgr->SetDevice(device);
    layer_mgr->SetScreen(_Screen->GetSurface());

    SetLayerManagerPositions();

    return layer_mgr;
}


void HandServer::SetLayerManagerPositions()
{
    LayerManager* lm;
    vector<LayerManager*>::iterator curr;
    vector<LayerManager*>::iterator end = LayerMgrs.end();
    int i = 0;
    SDL_Rect screen = _Screen->GetResolution();
    SDL_Rect screen_tmp = screen;
    int nr_of_lms = LayerMgrs.size();
    for(curr=LayerMgrs.begin(); curr!=end; curr++)
    {
        lm = (*curr);
        screen_tmp.w = screen.w/nr_of_lms;
        screen_tmp.x = screen_tmp.w*i;
        lm->SetSize(screen_tmp);
        i++;
    }
}


bool HandServer::Present(string file)
{
    Vertex* app = Produce(new Note("Command line input", file), "");
    if(app && app->IsType(HANDAPP))
    {
        LayerMgrs[0]->LoadAppInterface(app, true);
        return true;
    }
    return false;
}


// -----------------------------------------------
// Device Manager


// C Methods

// Removes all devices but the keyboard from the event queue
int EventFilter(const SDL_Event *event)
{
    if((event->type == SDL_KEYDOWN) || (event->type == SDL_KEYUP))
        return 1;
    return 0;
}

// C++ methods can't be called directly as a callback
Uint32 CallServerMethod(Uint32 i, void* server)
{
    HandServer* hand = reinterpret_cast<HandServer*>(server);
    hand->Pump();
    return i;
}


int HandServer::Register(Device* d)
{
    Devices.push_back(d);
    endDevice = Devices.end();
    if(Devices.size() == 1)
    {
//        DataManager::Validate(d->App_Interface());
    }
    return Devices.size();
}


void HandServer::Unregister(Device* d)
{
    Device* tmp;
    for(currentDevice=Devices.begin(); currentDevice!=endDevice; currentDevice++)
    {
        tmp = (*currentDevice);
        if(tmp == d)
        {
            currentDevice = Devices.erase(currentDevice);
            delete tmp;
            endDevice = Devices.end();
            return;
        }
    }
}


void HandServer::Beat()
{
    // Start only once
    if(Timer != NULL)
        return;
    // Only for speed
    SDL_SetEventFilter(EventFilter);
    // 25 pix per sec
    Uint32 interval = 1000/25;
    Timer = SDL_AddTimer(interval, &CallServerMethod, (void*)(this));
    if(Timer == NULL)
        exit(1);
    // Stop the main execution line
    pause();
}


void HandServer::StopBeating()
{
    // TODO: probably never ever used
    if(Timer != NULL)
        SDL_RemoveTimer(Timer);
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
    for(currentDevice=Devices.begin(); currentDevice!=endDevice; currentDevice++)
        if((*currentDevice)->Press(k))
            return;

    if(CreateNewUserOnOrphanKeyPress)
    {
        // Create a new device if the pressed key doesn't fits to an existing one
        LayerManager* lm = GetLayerManager();
        Device* dev = lm->GetDevice();
        dev->Press(k);
    }
}


void HandServer::Release(SDLKey k)
{
    // Gets the device
    for(currentDevice=Devices.begin(); currentDevice!=endDevice; currentDevice++)
    {
        if((*currentDevice)->Release(k))
        {
            if(DeleteDeviceIfEmpty
               && (*currentDevice)->IsUnused()
               && (LayerMgrs.size() > 1))
            {
                Exit(LayerMgrs.back());
            }
            return;
        }
    }
}
// End Device Manager
