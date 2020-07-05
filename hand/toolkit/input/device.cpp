#include "input/device.h"
#include "input/inputstate.h"


// C Method, removes all devices but the keyboard from the event queue.
int EventFilter(const SDL_Event *event)
{
    if((event->type == SDL_KEYDOWN) || (event->type == SDL_KEYUP))
        return 1;
    return 0;
}



Device::Device() : Module()
{
    // Only for speed
    SDL_SetEventFilter(EventFilter);
    m_KeysHmi = new Collection("Keyboard Initialization", "Press 5 keys on the keyboard");
    m_KeysHmi->Add(new Note("Thumb", "", ""));
    m_KeysHmi->Add(new Note("Pointer finger", "", ""));
    m_KeysHmi->Add(new Note("Middle finger", "", ""));
    m_KeysHmi->Add(new Note("Ring finger", "", ""));
    m_KeysHmi->Add(new Note("Little finger", "", ""));
}


Device::~Device()
{
    delete m_StateMachine;
    delete m_KeysHmi;
}


Interface* Device::GetHmi()
{
    if (!m_Hmi)
    {
        m_Hmi = new Interface("Input device", "Keyboard initialization");
        m_Hmi->SetControls(m_KeysHmi);
    }
    return m_Hmi;
}


bool Device::Init()
{
    for (unsigned i = 0; i < m_NumberOfKeys; ++i)
    {
        // Do we have keys to load?
        Note* data = GetKey(i);
        if(!data)
            return false;
        std::string key_str = data->GetValue();
        if(key_str.empty())
            return false;
        // Translate list entries to device keys
        m_Keys.push_back(atoi(key_str.c_str()));
    }
    return true;
}


void Device::GetUserInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_KEYDOWN:
            Press(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            Release(event.key.keysym.sym);
            break;
        }
    }
}


bool Device::Press(int k)
{
    if (m_Keys.size() < m_NumberOfKeys)
        // Device is during initialization
        AddKey(k);

    int index = GetKeyIndex(k);
    if(index == -1)
        return false;

    m_StateMachine->Press(index);
    return true;
}


bool Device::Release(int k)
{
    int index = GetKeyIndex(k);
    if (index == -1)
        return false;

    if (m_Keys.size() < m_NumberOfKeys)
    {
        // Device is during initialization
        DeleteKey(index);
        m_StateMachine->reset();
        for (unsigned i = 0; i < m_Keys.size(); ++i)
            m_StateMachine->Press(i);
        return true;
    }
    return m_StateMachine->Release(index);
}


int Device::GetKeyIndex(int k)
{
    for (int i = 0; i < m_Keys.size(); ++i)
        if (m_Keys[i] == k)
            return i;

    return -1;
}


Note* Device::GetKey(unsigned pos)
{
    return dynamic_cast<Note*>(m_KeysHmi->GetChild(pos));
}


bool Device::IsUnused()
{
    return !m_Keys.size();
}


void Device::AddKey(int k)
{
    int index = m_Keys.size();
    m_Keys.push_back(k);

    GetKey(index)->SetValue(SDL_GetKeyName((SDLKey)k));
}


void Device::DeleteKey(unsigned index)
{
    // Adjust UI literals
    Note* curr = GetKey(index);
    for (unsigned i = index + 1; i < m_NumberOfKeys; ++i)
    {
        Note* next = GetKey(i);
        curr->SetValue(next->GetValue());
        curr = next;
    }

    // Delete from the SDL key map
    m_Keys.erase(m_Keys.begin() + index);
}


unsigned Device::GetNumberOfKeys()
{
    return m_NumberOfKeys;
}


InputState* Device::GetInputState()
{
    // Create inputstate object
    if(!m_StateMachine)
        m_StateMachine = new InputState(m_NumberOfKeys);
    return m_StateMachine;
}
