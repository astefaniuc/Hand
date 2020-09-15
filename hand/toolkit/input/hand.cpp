#include "input/hand.h"
#include "input/device.h"
#include "input/inputstate.h"
#include "graph/interface.h"
#include "view/layer/layer.h"


Hand::Hand(Device* dev) : Module(), m_Device(dev)
{
    m_Device->SetUser(this);
    m_KeysHmi = new Collection("Keyboard Initialization", "Press 5 keys on the keyboard");
    m_KeysHmi->Add(new Note("Thumb", "", ""));
    m_KeysHmi->Add(new Note("Pointer finger", "", ""));
    m_KeysHmi->Add(new Note("Middle finger", "", ""));
    m_KeysHmi->Add(new Note("Ring finger", "", ""));
    m_KeysHmi->Add(new Note("Little finger", "", ""));
    ((ListLayout*)m_KeysHmi->GetLayer()->GetLayout())->SetAlignment(ListLayout::Horizontal);
    m_KeysHmi->AddActivationClient(new CCallback<Layer>(m_KeysHmi->GetLayer(), &Layer::Exit));
    m_StateMachine = new InputState(m_NumberOfKeys);
}


Hand::~Hand()
{
    delete m_StateMachine;
    delete m_KeysHmi;
}


HmiItem* Hand::GetHmi()
{
    GetInputState()->GetCommand(m_KeysHmi, m_NumberOfKeys);
    return m_KeysHmi;
}


bool Hand::Init()
{
    for (unsigned i = 0; i < m_NumberOfKeys; ++i)
    {
        // Do we have keys to load?
        Note* data = GetKey(i);
        if (!data)
            return false;
        std::string key_str = data->GetValue();
        if (key_str.empty())
            return false;
        // Translate list entries to device keys
        m_Keys.push_back(atoi(key_str.c_str()));
    }
    return true;
}


bool Hand::Press(int k)
{
    if (m_Keys.size() < m_NumberOfKeys)
        // Device is during initialization
        AddKey(k);

    int index = GetKeyIndex(k);
    if (index == -1)
        return false;

    m_StateMachine->Press(index);
    return true;
}


bool Hand::Release(int k)
{
    int index = GetKeyIndex(k);
    if (index == -1)
        return false;

    if (m_Keys.size() < m_NumberOfKeys)
    {
        // Device is during initialization
        DeleteKey(index);
        m_StateMachine->Reset();
        for (unsigned i = 0; i < m_Keys.size(); ++i)
            m_StateMachine->Press(i);
        return true;
    }
    return m_StateMachine->Release(index);
}


int Hand::GetKeyIndex(int k)
{
    for (int i = 0; i < m_Keys.size(); ++i)
        if (m_Keys[i] == k)
            return i;

    return -1;
}


Note* Hand::GetKey(unsigned pos)
{
    return dynamic_cast<Note*>(m_KeysHmi->GetChild(pos));
}


void Hand::AddKey(int k)
{
    int index = m_Keys.size();
    m_Keys.push_back(k);
    m_Device->GetKeyName(k, GetKey(index));
}


void Hand::DeleteKey(unsigned index)
{
    // Adjust UI literals
    Note* curr = GetKey(index);
    for (unsigned i = index + 1; i < m_NumberOfKeys; ++i)
    {
        Note* next = GetKey(i);
        curr->SetValue(next->GetValue());
        curr = next;
    }

    // Delete from the device key map
    m_Keys.erase(m_Keys.begin() + index);
}
