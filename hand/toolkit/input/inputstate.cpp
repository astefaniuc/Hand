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

#include "input/inputstate.h"
#include "graph/vertex.h"
#include "view/layer/layermanager.h"


using namespace std;


InputState::InputState(Vertex* dev)
{
    LM = dynamic_cast<LayerManager*>(dev->Vertex::get(LAYERMANAGER)->get());
    dev->get(FACTORY, STATEGRAPH)->execute(dev);
    NullKey = PressedKey = ReleasedKey = dynamic_cast<StateNode*>(dev->Vertex::get(STATENODE, ANY));
    LM->set(GetCommands(1));
}


bool InputState::Press(uint k)
{
    PressedKey = ReleasedKey = ReleasedKey->GetChild(k);
    if(PressedKey == NULL)
    {
        reset();
        return false;
    }

    return true;
}


bool InputState::Release(uint k)
{
    // Move the ReleasedKey up the the three
    // and the ReleasedKey down to the current key combination node
    if((PressedKey==NullKey) ||
        ((ReleasedKey=ReleasedKey->GetParent(k)) == NULL))
    {
        // Some events get lost, this command is corrupted
        reset();
        return false;
    }

    // Execute command when all pressed keys have been released
    if(ReleasedKey == NullKey)
    {
        // Inform the GUI
        Vertex* method = PressedKey->Vertex::get(METHOD)->get();
        if(method)
            method->execute(method);

        reset();
    }
    return true;
}


void InputState::reset(void)
{
    PressedKey = ReleasedKey = NullKey;
}


StateNode* InputState::GetKey(key_pointer key)
{
    switch(key)
    {
        case ROOT:
            return NullKey;
        case PRESSED:
            return PressedKey;
        case RELEASED:
            return ReleasedKey;
        default:
            return NULL;
    }
}


bool InputState::GetCommand(Vertex* target, uint level)
{
    if(NullKey == NULL)
        // Not initialized
        return false;

    // Get the list of available commands
    Vertex* free_cmds = GetCommands(level);
    Vertex* f_cmd;
    uint i = 0;
    while((f_cmd=free_cmds->get(++i)) != NULL)
    {
        // Bind the functional list to the state graph (command) node,
        // bidirectional
        if(f_cmd->Vertex::get(METHOD)->set(target))
        {
            target->get(COMMAND)->set(f_cmd);

            return true;
        }
    }
    return false;
}


Vertex* InputState::GetCommands(uint level)
{
    if(level > NullKey->size())
        return NULL;

    // Search from bottom up
    StateNode* tmp_node = NullKey;
    for(uint i=1; i<=level; i++)
        tmp_node = tmp_node->GetChild(i);

    return tmp_node->Vertex::get(COMMANDS);
}
