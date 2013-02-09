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

#include <vector>
#include "base/handapp.h"
#include "graph/data.h"


class InputState;

class Device : public virtual HandApp
{
    public:
               Device();
              ~Device();

        using HandApp::get;
        // Implements get(VIEW)
        Vertex* get(std::string name);
        // Sets the key map
        bool   Init();
        // Functions accessing the layer object (not possible from here)
        bool   Press(int key_id);
        bool   Release(int key_id);
        bool   IsUnused();
        uint   GetNumberOfKeys();
        // Return the input state machine
        InputState* GetInputState();
    protected:
        // Returns the Key symbol at specified position
        Note*  GetKey(uint index);
        void   AddKey(int key_id);
        void   DeleteKey(uint index);
        // Returns the key number
        int    GetKeyIndex(int key_id);

        // Members:
        InputState* StateMachine;
        // Number of controls
        uint numberOfKeys;
        std::vector<int> Keys;
        std::vector<int>::iterator currentKey;
};

#endif /* INPUT_DEVICE_H */
