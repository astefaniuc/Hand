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
    // Deletes keys
    for(currentKey=Keys.begin(); currentKey!=Keys.end(); currentKey++)
        delete(*currentKey);

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
    if(index == -1)
        return false;

    if(Keys.size() < numberOfKeys)
    {
        // Device is during initialization
        DeleteKey(index);
        StateMachine->reset();
        for(uint i=0; i<Keys.size(); ++i)
            StateMachine->Press(i);
        return true;
    }
    return StateMachine->Release(index);
}


int Device::GetKeyIndex(SDLKey k)
{
    int i = 0;
    for(currentKey=Keys.begin(); currentKey!=Keys.end(); currentKey++, i++)
        if(*(*currentKey) == k)
            return i;

    return -1;
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
    // Display the key on the initialization screen
    int index = GetKeyIndex(k);
    currentKey = Keys.begin() + index;

    GetKey(index+1)->set(SDL_GetKeyName(*(*currentKey)));
}


void Device::DeleteKey(uint index)
{
    Note* curr;
    Note* next;
    uint i = index;
    string key;
    while((curr=GetKey(++i)) != NULL)
    {
        next = GetKey(i+1);
        if(next)
            key = next->get();
        else
            key = "";
        curr->set(key);
    }

    currentKey = Keys.begin() + index;
    if(currentKey == Keys.end())
        return;

    SDLKey* tmp = (*currentKey);
    currentKey = Keys.erase(currentKey);
    delete tmp;
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
