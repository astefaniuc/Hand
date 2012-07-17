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

#ifndef HAND_DEVICE_H
#define HAND_DEVICE_H

#include "handapp.h"


using namespace std;


class InputState;

class Device : public virtual HandApp
{
    typedef vector<SDLKey*> keygroup;

    public:
               Device();
              ~Device();
        // Sets the key map
        bool   Init();
        //virtual void Register(void);
        void   AddKey(int);
        // Functions accessing the layer object (not possible from here)
        bool   Press(SDLKey);
        bool   Release(SDLKey);
        bool   IsUnused();
        uint   GetNumberOfKeys();
        // Return the input state machine
        InputState* GetInputState();
    protected:
        // Returns the Key symbol at specified position
        FunctoidNode* GetKey(uint index);
        string GetKeyName(uint pos);
        void   AddKey(SDLKey);
        void   DeleteCurrentKey();
        // Returns the key number
        int    GetKeyIndex(SDLKey);
        int    GetInitializationIndex(SDLKey);

        bool   ShowPressedKey(Functoid*);

        // Members:
        InputState* StateMachine;
        // Number of controls
        uint numberOfKeys;
        keygroup Keys;
        keygroup KeysInit;
        keygroup::iterator currentKey;
        keygroup::iterator keysEnd;
};

#define DEVICE_KEYLIST "Keylist"

#endif /* HAND_DEVICE_H */
