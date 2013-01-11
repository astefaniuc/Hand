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

#include <vector>
#include <string>
#include "input/node.h"
#include "include/defines.h"


class Device;
class LayerManager;

class InputState
{

    public:
        // Link to the parent object, the main server
        InputState(Vertex* device);
        virtual ~InputState(){};
        // Stores information about pressed keys by moving up the PressedKey ptr
        bool Press(uint index);
        // Stores information about released keys by moving down the ReleasedKey ptr
        bool Release(uint index);
        // Resets "pressed" and "released" information
        void reset();
        StateNode* GetKey(key_pointer);
        Vertex* GetCommands(uint level);
        // Maps a command node to the given list
        bool GetCommand(Vertex* method, uint level);

    protected:
        LayerManager* LM;

        StateNode* NullKey;
        StateNode* PressedKey;
        StateNode* ReleasedKey;
};

#endif /* INPUT_INPUTSTATE_H */
