#include "input/hand.h"
#include "input/device.h"
#include "input/inputstate.h"
#include "data/interface.h"
#include "view/layer.h"
#include "view/layers/list.h"
#include "view/layouts/placed.h"
#include "view/layouts/aligned.h"


Hand::Hand(Device* dev) : Module(), m_Device(dev)
{
    m_Device->SetUser(this);
    m_KeysHmi = new Collection("Keys", "");
    m_KeysHmi->Add(new Note("Thumb", "", ""));
    m_KeysHmi->Add(new Note("Pointer finger", "", ""));
    m_KeysHmi->Add(new Note("Middle finger", "", ""));
    m_KeysHmi->Add(new Note("Ring finger", "", ""));
    m_KeysHmi->Add(new Note("Little finger", "", ""));

    m_StateMachine = new InputState(m_NumberOfKeys);
}


Hand::~Hand()
{
    delete m_StateMachine;
    delete m_KeysHmi;
    delete m_InitScreen;
}


HmiItem* Hand::GetHmi()
{
    if (!m_InitScreen)
    {
        Layouts::Placed::List* handLayout = new Layouts::Placed::List();
        handLayout->SetField("Thumb", { 0.21, 0.6, 0.0, 0.0 });
        handLayout->SetField("Pointer finger", { 0.3, 0.4, 0.0, 0.0 });
        handLayout->SetField("Middle finger", { 0.45, 0.39, 0.0, 0.0 });
        handLayout->SetField("Ring finger", { 0.6, 0.41, 0.0, 0.0 });
        handLayout->SetField("Little finger", { 0.7, 0.5, 0.0, 0.0 });
        m_KeysHmi->GetLayer()->SetLayout(handLayout);
        static_cast<Layers::List*>(m_KeysHmi->GetLayer())->SetExpandChildren(true);

        Layouts::Aligned::Map* screenLayout = Layouts::Aligned::CreateView();
        screenLayout->SetField(DESCRIPTION, { Layout::Bottom, Layout::Center });

        m_InitScreen = new Interface(
            "Keyboard Initialization",
            "Press 5 keys on the keyboard, to initialize a Hand device.");
        m_InitScreen->SetView(m_KeysHmi->GetLayer());
        m_InitScreen->GetLayer()->SetLayout(screenLayout);

        m_KeysHmi->AddActivationClient(
            new CCallback<Layer>(m_InitScreen->GetLayer(), &Layer::Exit));
        GetInputState()->GetCommand(m_KeysHmi, m_NumberOfKeys);
    }

    return m_InitScreen;
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
    return static_cast<Note*>(m_KeysHmi->GetChild(pos));
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
