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

#ifndef INPUT_INPUTSTATE_H
#define INPUT_INPUTSTATE_H

#include "input/node.h"
#include "include/defines.h"
#include <vector>
#include <string>


class Device;
class StateTree;
class Layer;

class InputState
{

    public:
        // Link to the parent object, the main server
        InputState(Device*);
        ~InputState();
        // Stores information about pressed keys by moving up the PressedKey ptr
        bool Press(int index);
        // Stores information about released keys by moving down the ReleasedKey ptr
        bool Release(int index);
        // Resets "pressed" and "released" information
        void reset();
        // Returns the number of controls set
        uint  GetNumberOfKeys();
        Node* GetKey(key_pointer);
        std::vector<Node*>* GetLevelVector(int);
        // Returns the pointer to the associated device
        Device* GetDevice();
        void SetDevice(Device*);
        // Maps a command node to the given list
        bool GetCommand(Layer*, int);

    protected:
        Device* InputDevice;
        // Stores the root node as entry point
        StateTree* STree;

    private:
        // Members:
        Node* NullKey;
        Node* PressedKey;
        Node* FollowerKey;
};

#endif /* INPUT_INPUTSTATE_H */
