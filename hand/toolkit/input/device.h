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

#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include <SDL/SDL.h>
#include <vector>
#include "base/handapp.h"
#include "graph/data.h"


class InputState;

class Device : public virtual HandApp
{
    typedef std::vector<SDLKey*> keygroup;

    public:
               Device();
              ~Device();
        // Sets the key map
        bool   Init();
        bool   execute(Vertex* init_screen);
        // Functions accessing the layer object (not possible from here)
        bool   Press(SDLKey);
        bool   Release(SDLKey);
        bool   IsUnused();
        uint   GetNumberOfKeys();
        // Return the input state machine
        InputState* GetInputState();
    protected:
        // Returns the Key symbol at specified position
        Note*  GetKey(uint index);
        void   AddKey(SDLKey);
        void   DeleteKey(uint index);
        // Returns the key number
        int    GetKeyIndex(SDLKey);
        int    GetInitializationIndex(SDLKey);

        // Members:
        InputState* StateMachine;
        // Number of controls
        uint numberOfKeys;
        keygroup Keys;
        keygroup KeysInit;
        keygroup::iterator currentKey;
};

#define DEVICE_KEYLIST "Keylist"

#endif /* INPUT_DEVICE_H */
