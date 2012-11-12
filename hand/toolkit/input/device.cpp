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

#include "input/device.h"
#include "input/inputstate.h"
#include "graph/method.h"


using namespace std;


Device::Device() : HandApp("settings:Keyboard::0")
{
    StateMachine = NULL;
    numberOfKeys = NUMBER_OF_BUTTONS;
}


Device::~Device()
{
    currentKey = Keys.begin();
    // Recursively deletes sub-layers
    while(currentKey!=Keys.end())
        DeleteCurrentKey();

    delete StateMachine;
}


bool Device::Init()
{
    string  key_str;
    SDLKey* key_sdl;
    Note* data;

    for (uint i=1; i<=numberOfKeys; i++)
    {
        // Do we have keys to load?
        data = GetKey(i);
        if(!data)
            return false;
        key_str = data->get();
        if(key_str == "")
            return false;
        // Translate list entries to device keys
        key_sdl = new SDLKey();
        *key_sdl = (SDLKey)atoi(key_str.c_str());
        Keys.push_back(key_sdl);
    }
    keysEnd = Keys.end();
    return true;
}


bool Device::execute(Vertex* init_screen)
{
    // Two main entries: a keylist and the description
    init_screen->add(new Note(DESCRIPTION, "Press 5 keys on the keyboard"));

    Vertex* keys_data_tree = Vertex::get(DEVICE_KEYLIST);
    init_screen->attach(keys_data_tree);

    // Load un-initialized keys
    Vertex* id;
    for(int i=0; i < NUMBER_OF_BUTTONS; ++i)
    {
        id = new Note("Keydata", "");

        keys_data_tree->add(id);
    }
    return true;
}


bool Device::Press(SDLKey k)
{
    if(Keys.size() < numberOfKeys)
        // Device is during initialization
        AddKey(k);

    int index = GetKeyIndex(k);
    if(index == -1)
        return false;

    StateMachine->Press(index);
    return true;
}


bool Device::Release(SDLKey k)
{
    int index = GetKeyIndex(k);
    if(index != -1)
    {
        if(Keys.size() < numberOfKeys)
        {
            // Device is during initialization
            index = GetInitializationIndex(k);
            if(index != -1)
                DeleteCurrentKey();
        }
        StateMachine->Release(index);
        return true;
    }
    return false;
}


int Device::GetKeyIndex(SDLKey k)
{
    int i = 0;
    for(currentKey=Keys.begin(); currentKey!=keysEnd; currentKey++, i++)
        if(*(*currentKey) == k)
            return i;

    return 0;
}


Note* Device::GetKey(uint pos)
{
    return dynamic_cast<Note*>(Vertex::get(DEVICE_KEYLIST)->get(pos));
}


bool Device::IsUnused()
{
    if(Keys.size() == 0)
        return true;
    return false;
}


void Device::AddKey(SDLKey k)
{
    SDLKey* tmp = new SDLKey();
    *tmp = k;
    Keys.push_back(tmp);
    keysEnd = Keys.end();
    KeysInit = Keys;
    // Display the key on the initialization screen
    int index = GetKeyIndex(k);
    currentKey = Keys.begin() + index;

    GetKey(index+1)->set(SDL_GetKeyName(*(*currentKey)));
}


void Device::DeleteCurrentKey()
{
    SDLKey* tmp = (*currentKey);
    currentKey = Keys.erase(currentKey);
    delete tmp;
    keysEnd = Keys.end();
    // Removes the key from the initialization screen
    // TODO: check this removal of last key (should be the one removed?)
    GetKey(Keys.size()+1)->set("");
}


int Device::GetInitializationIndex(SDLKey key)
{
    keygroup::iterator k;
    keygroup::iterator k_end = KeysInit.end();
    int i = 0;
    for(k=KeysInit.begin(); k!=k_end; k++, i++)
        if(*(*k) == key)
            return i;

    return -1;
}


uint Device::GetNumberOfKeys()
{
    return numberOfKeys;
}


InputState* Device::GetInputState()
{
    // Create inputstate object
    if(StateMachine == NULL)
        StateMachine = new InputState(this);
    return StateMachine;
}
