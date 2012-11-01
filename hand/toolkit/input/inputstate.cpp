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
#include "input/statetree.h"
#include "input/device.h"
#include "graph/vertex.h"
#include "view/layer/layer.h"


using namespace std;


InputState::InputState(Device* d)
{
    SetDevice(d);
    // TODO: check the size of the existing tree, if it doesn't matches
    // create a new one and store it in a vector in the server
    STree = new StateTree(InputDevice->GetNumberOfKeys());
    NullKey = PressedKey = FollowerKey = STree->GetEntryPoint();
}


InputState::~InputState()
{
    delete(STree);
}


Device* InputState::GetDevice()
{
    return InputDevice;
}


void InputState::SetDevice(Device* d)
{
    InputDevice = d;
}


bool InputState::Press(int k)
{
    PressedKey = FollowerKey = FollowerKey->GetChild(k);
    if(PressedKey == NULL)
    {
        reset();
        return false;
    }

    // Inform the GUI
    Layer* layer = PressedKey->GetLayer();
    if(layer)
        // There is a GUI item mapped to this command
        layer->PressGui();
    else
    {
        // Inform the LayerManager
        //ShowAvailableCommands();
    }
    return true;
}


bool InputState::Release(int k)
{
    // Move the ReleasedKey up the the three
    // and the FollowerKey down to the current key combination node
    if((PressedKey==NullKey) ||
        ((FollowerKey=FollowerKey->GetParent(k)) == NULL))
    {
        // Some events get lost, this command is corrupted
        reset();
        return false;
    }

    // Execute command when all pressed keys have been released
    if(FollowerKey == NullKey)
    {
        // Inform the GUI
        Layer* layer = PressedKey->GetLayer();
        if(layer)
            // There is a GUI item mapped to this command
            layer->ReleaseGui();

        reset();
    }
    else
    {
        // Inform the LayerManager
        //ShowAvailableCommands();
    }
    return true;
}


void InputState::reset(void)
{
    PressedKey = FollowerKey = NullKey;
}


Node* InputState::GetKey(key_pointer key)
{
    switch(key)
    {
        case ROOT:
            return NullKey;
        case PRESSED:
            return PressedKey;
        case FOLLOWER:
            return FollowerKey;
        default:
            return NULL;
    }
}


bool InputState::GetCommand(Layer* target, int level)
{
    if(NullKey == NULL)
        // Not initialized
        return false;

    // Get the list of available commands
    vector<Node*>* free_cmds = GetLevelVector(level);
    vector<Node*>::iterator f_cmd;
    for(f_cmd=free_cmds->begin(); f_cmd!=free_cmds->end(); f_cmd++)
    {
        // Bind the functional list to the statetree (command) node,
        // bidirectional
        if((*f_cmd)->SetLayer(target))
        {
            target->SetCommand((*f_cmd));
            return true;
        }
    }
    return false;
}


vector<Node*>* InputState::GetLevelVector(int level)
{
    if(level > NullKey->GetSize())
        return NULL;

    // Search from bottom up
    Node* tmp_node = NullKey;
    for(int i=0; i<level; i++)
        tmp_node = tmp_node->GetChild(i);

    return tmp_node->GetPeerNodes();
}


uint InputState::GetNumberOfKeys()
{
    if(InputDevice != NULL)
        return InputDevice->GetNumberOfKeys();
    return 0;
}
