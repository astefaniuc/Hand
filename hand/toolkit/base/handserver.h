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

#ifndef BASE_HANDSERVER_H
#define BASE_HANDSERVER_H

#include <SDL/SDL.h>
#include "base/datamanager.h"


class LayerManager;
class Device;
class Screen;

class HandServer : public DataManager
{
    // Singleton
    private:
        HandServer();
        ~HandServer();

    public:
        static HandServer* GetInstance(void);
        // Called from main()
        void Present(std::string file);
    private:
        // Initializes all subsystems:
        LayerManager* GetUser();
        // Starts timer (infinite loop)
        void Beat();
    public:
        // Called from C callback
        void Pump();
    private:
        //  User input handling
        void GetUserInput(void);
        void Press(SDLKey);
        void Release(SDLKey);

        // Members:
        // The one and only server (singleton)
        static HandServer* Instance;
        // TODO: multiple screens e.g. touch screen device
        Screen* _Screen;
        // The queue for system device events
        SDL_Event Event;
        SDL_TimerID Timer;
        bool ExecNotFinished;

        // Variables controlling the server behavior - "settings"
        bool CreateNewUserOnOrphanKeyPress;
        bool DeleteDeviceIfEmpty;
};

#endif /* BASE_HANDSERVER_H */
