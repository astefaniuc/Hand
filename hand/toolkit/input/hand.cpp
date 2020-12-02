#include "input/hand.h"
#include "input/chord.h"
#include "input/device.h"
#include "input/inputstate.h"
#include "data/interface.h"
#include "view/layer.h"
#include "view/layers/vector.h"
#include "view/layouts/placed.h"
#include "view/layouts/aligned.h"


const std::string Hand::Finger[] = {
    "Thumb", "Pointer finger", "Middle finger", "Ring finger", "Little finger" };


Hand::Hand(Device* dev) : Module(), m_Device(dev)
{
    m_Device->SetUser(this);
    m_KeysHmi = new Hmi::Vector("Keys", "");
    m_KeysHmi->Add(new Hmi::Note(Finger[Chord::Thumb], "", ""));
    m_KeysHmi->Add(new Hmi::Note(Finger[Chord::Pointer], "", ""));
    m_KeysHmi->Add(new Hmi::Note(Finger[Chord::Middle], "", ""));
    m_KeysHmi->Add(new Hmi::Note(Finger[Chord::Ring], "", ""));
    m_KeysHmi->Add(new Hmi::Note(Finger[Chord::Little], "", ""));

    m_InputState = new InputState(m_NumberOfKeys);
}


Hand::~Hand()
{
    delete m_InputState;
    delete m_KeysHmi;
    delete m_InitScreen;
}


Hmi::Item* Hand::GetInitScreen()
{
    if (!m_InitScreen)
    {
        Layouts::Placed::List* handLayout = new Layouts::Placed::List();
        handLayout->SetField("Thumb", { 0.21, 0.6, 0.0, 0.0 });
        handLayout->SetField("Pointer finger", { 0.3, 0.4, 0.0, 0.0 });
        handLayout->SetField("Middle finger", { 0.45, 0.39, 0.0, 0.0 });
        handLayout->SetField("Ring finger", { 0.6, 0.41, 0.0, 0.0 });
        handLayout->SetField("Little finger", { 0.7, 0.5, 0.0, 0.0 });

        Layers::Vector* handLayer = static_cast<Layers::Vector*>(m_KeysHmi->GetExpandedView());
        handLayer->SetLayout(handLayout);
        handLayer->SetExpandChildren(true);

        Layouts::Aligned::Map* screenLayout = Layouts::Aligned::CreateView();
        screenLayout->SetField(DESCRIPTION, { Layouts::Bottom, Layouts::Center });
        screenLayout->GetField(CONTROL)->SetVisible(false);
        screenLayout->GetField(LAYER_CONTROLS)->SetVisible(false);

        m_InitScreen = new Hmi::Interface(
            "Keyboard Initialization",
            "Press 5 keys on the keyboard, to initialize a Hand device.");
        m_InitScreen->SetView(m_KeysHmi);

        static_cast<Layers::List*>(m_InitScreen->GetExpandedView())->SetLayout(screenLayout);
    }

    return m_InitScreen;
}


bool Hand::Init()
{
    for (unsigned i = 0; i < m_NumberOfKeys; ++i)
    {
        // Do we have keys to load?
        Hmi::Note* data = GetKey(i);
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


bool Hand::SetFocus(Layer* view)
{
    m_Commands.clear();
    BindChords(view);
    return true;
}

void Hand::BindChords(Layer* focus)
{
    Hmi::Item* method = focus->GetContent();
    if (method && !method->m_Chord.keys.empty())
        m_Commands[method] = method->m_Chord;
/*
    Layer* sub = focus->GetFirstChild();
    while (sub)
    {
        BindChords(sub);
        sub = focus->GetNextChild();
    }
    */
}


bool Hand::Press(int k)
{
    if (m_Keys.size() < m_NumberOfKeys)
        // Device is during initialization
        AddKey(k);

    int index = GetKeyIndex(k);
    if (index == -1)
        return false;

    if (m_InputState->IsClean())
        m_Record.keys.clear();
    if (m_InputState->Press(index))
        m_Record.keys.push_back((Chord::Finger)index);

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
        // Always ascending
        m_Record.keys.pop_back();
        m_InputState->Release(m_Keys.size());
        return true;
    }

    if (m_InputState->Release(index) && m_InputState->IsClean())
    {
        for (auto command : m_Commands)
        {
            if (command.second.IsValid(m_Record))
            {
                command.first->Activate();
                break;
            }
        }
    }

    return true;
}


int Hand::GetKeyIndex(int k)
{
    for (int i = 0; i < m_Keys.size(); ++i)
        if (m_Keys[i] == k)
            return i;

    return -1;
}


Hmi::Note* Hand::GetKey(unsigned pos)
{
    return static_cast<Hmi::Note*>(m_KeysHmi->GetChild(pos));
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
    Hmi::Note* curr = GetKey(index);
    for (unsigned i = index + 1; i < m_NumberOfKeys; ++i)
    {
        Hmi::Note* next = GetKey(i);
        curr->SetValue(next->GetValue());
        curr = next;
    }

    // Delete from the device key map
    m_Keys.erase(m_Keys.begin() + index);
}
