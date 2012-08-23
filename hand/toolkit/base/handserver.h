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

#ifndef HAND_HANDSERVER_H
#define HAND_HANDSERVER_H

#include "base/datamanager.h"


using namespace std;


class Layer;
class LayerManager;
class Device;
class Screen;

class HandServer : public virtual DataManager
{
    public:
        static HandServer* GetInstance(void);
        void Exit(Layer*);
        void Pump();

        // Adds device (keys) to the input poll
        int  Register(Device*);
        // Removes device from the input poll
        void Unregister(Device*);
        // Initializes all subsystems:
        LayerManager* GetLayerManager();
        // Re-calculate position for all layer manager
        void SetLayerManagerPositions();
        // Called from main() of stand-alone apps
        bool Present(string file);
        // Starts timer
        void Beat();

    private:
        HandServer();
        ~HandServer();

        // Starts polling and drawing
        void Start();
        // Stop the event polling endless loop from outside
        void StopBeating();

        // Infinite loop handling user input
        void GetUserInput(void);
        void Press(SDLKey);
        void Release(SDLKey);

        // Members:
        // The one and only server (singleton)
        static HandServer* Instance;
        vector<LayerManager*> LayerMgrs;
        // TODO: declare here iterator for LM as for Device
        vector<Device*> Devices;
        vector<Device*>::iterator currentDevice;
        vector<Device*>::iterator endDevice;
        // TODO: multiple drawers e.g. touch screen device
        Screen* _Screen;
        // The queue for system device events
        SDL_Event Event;
        SDL_TimerID Timer;
        bool ExecNotFinished;

        // Variables controlling the server behavior - "settings"
        bool CreateNewUserOnOrphanKeyPress;
        bool DeleteDeviceIfEmpty;
};

#endif /* HAND_HANDSERVER_H */
