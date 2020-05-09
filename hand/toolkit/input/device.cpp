#include <SDL/SDL.h>
#include "input/device.h"
#include "input/inputstate.h"
#include "graph/method.h"
#include "view/datatypes/layout.h"


Device::Device() : HandApp("settings:Keyboard::0")
{
}


Device::~Device()
{
    delete(StateMachine);
}


Vertex* Device::get(const std::string& name)
{
    Vertex* ret = List::get(ANY, name);
    if(ret || (name!=VIEW))
        return ret;

    ret = List::get(VIEW);
    // Two main entries: a keylist and the description
    ret->add(new Note(DESCRIPTION, "Press 5 keys on the keyboard"));

    Vertex* keys_data_tree = Vertex::get(KEYLIST);
    Vertex* keys_view_tree = ret->get(KEYLIST);

    keys_view_tree->Vertex::get(LAYOUT)->get(LIST)->set(
            new LayoutFactory<Device>(LIST, this, &Device::GetKeyListLayout));

    // Load un-initialized keys
    Vertex* factory = Vertex::get(FACTORY, THEMES)->get(DEFAULT)
                            ->get(LAYOUT)->get(LIST)->get(FRAMEDLIST);
    Vertex* id;
    Vertex* frame;
    for(unsigned i=0; i < numberOfKeys; ++i)
    {
        id = new Note("Keydata", "");
        keys_data_tree->add(id);

        frame = new List("");
        frame->Vertex::get(LAYOUT)->get(LIST)->set(factory);
        frame->add(id);
        keys_view_tree->add(frame);
    }
    return ret;
}


bool Device::GetKeyListLayout(Vertex* layout)
{
    if(Vertex::get(FACTORY, THEMES)->get(DEFAULT)->get(LAYOUT)->get(LIST)->execute(layout))
    {
        layout->get(COORDINATES)->Vertex::get(REQUEST)->get(RECT)->get()->name(SCALED);
        layout->get(ALIGNMENT)->Vertex::get(REQUEST)->get(ALIGNMENT)->get()->name(HORIZONTAL);
        return true;
    }
    return false;
}


bool Device::Init()
{
    for(unsigned i = 1; i<=numberOfKeys; ++i)
    {
        // Do we have keys to load?
        Note* data = GetKey(i);
        if(!data)
            return false;
        std::string key_str = data->get();
        if(key_str.empty())
            return false;
        // Translate list entries to device keys
        Keys.push_back(atoi(key_str.c_str()));
    }
    return true;
}


bool Device::Press(int k)
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


bool Device::Release(int k)
{
    int index = GetKeyIndex(k);
    if(index == -1)
        return false;

    if(Keys.size() < numberOfKeys)
    {
        // Device is during initialization
        DeleteKey(index);
        StateMachine->reset();
        for(unsigned i=1; i<=Keys.size(); ++i)
            StateMachine->Press(i);
        return true;
    }
    return StateMachine->Release(index);
}


int Device::GetKeyIndex(int k)
{
    int i = 1;
    for(currentKey=Keys.begin(); currentKey!=Keys.end(); currentKey++, i++)
        if((*currentKey) == k)
            return i;

    return -1;
}


Note* Device::GetKey(unsigned pos)
{
    return dynamic_cast<Note*>(Vertex::get(KEYLIST)->get(pos));
}


bool Device::IsUnused()
{
    return !Keys.size();
}


void Device::AddKey(int k)
{
    Keys.push_back(k);
    // Display the key on the initialization screen
    int index = GetKeyIndex(k);
    currentKey = Keys.begin() + index - 1;

    GetKey(index)->set(SDL_GetKeyName((SDLKey)(*currentKey)));
}


void Device::DeleteKey(unsigned index)
{
    Note* curr;
    Note* next;
    unsigned i = index;
    std::string key;
    while((curr=GetKey(i)) != nullptr)
    {
        next = GetKey(++i);
        if(next)
            key = next->get();
        else
            key = "";
        curr->set(key);
    }

    currentKey = Keys.begin() + index - 1;
    if(currentKey != Keys.end())
        currentKey = Keys.erase(currentKey);
}


unsigned Device::GetNumberOfKeys()
{
    return numberOfKeys;
}


InputState* Device::GetInputState()
{
    // Create inputstate object
    if(!StateMachine)
        StateMachine = new InputState(this);
    return StateMachine;
}
