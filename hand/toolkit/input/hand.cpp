#include "input/hand.h"
#include "input/device.h"
#include "input/inputstate.h"
#include "data/interface.h"
#include "view/layers/hmi/interface.h"
#include "view/layers/vector.h"
#include "view/layouts/placed.h"
#include "view/layouts/builtin.h"
#include <algorithm>


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
}


Layer* Hand::GetInitScreen()
{
    Hmi::Interface* initScreen = new Hmi::Interface(
        "Keyboard Initialization",
        "Press 5 keys on the keyboard, to initialize a Hand device.");
    initScreen->GetView()->Attach(m_KeysHmi);
    m_KeysHmi->SetLayerInitializer(this, &Hand::InitHandLayer);
    initScreen->SetLayerInitializer(this, &Hand::InitInitLayer);

    Layers::Interface* ret = initScreen->GetExpandedView()->GetInterface();
    ret->ExitListeners.Add(initScreen, &Hmi::Interface::DeleteCb);
    return ret;
}


void Hand::InitInitLayer(Layer* l)
{
    Layout* layout = Layouts::CreateView();
    layout->GetField(DESCRIPTION)->SetAlignment(VAlignment::Bottom, HAlignment::HCenter);
    layout->GetField(CONTROL)->SetVisible(false);
    layout->GetField(LAYER_CONTROLS)->SetVisible(false);

    static_cast<Layers::List*>(l)->SetLayout(layout);
}


void Hand::InitHandLayer(Layer* l)
{
    Layouts::Placed::Map* layout = new Layouts::Placed::Map();
    layout->SetField(Finger[Chord::Thumb], { 0.21, 0.6, 0.0, 0.0 });
    layout->SetField(Finger[Chord::Pointer], { 0.3, 0.4, 0.0, 0.0 });
    layout->SetField(Finger[Chord::Middle], { 0.45, 0.39, 0.0, 0.0 });
    layout->SetField(Finger[Chord::Ring], { 0.6, 0.41, 0.0, 0.0 });
    layout->SetField(Finger[Chord::Little], { 0.7, 0.5, 0.0, 0.0 });

    Layers::Vector* lv = static_cast<Layers::Vector*>(l);
    lv->SetLayout(layout);
    lv->SetExpandChildren(true);
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


Chord* Hand::GetFreeChord(InteractionLevel interaction)
{
    unsigned lower = 1;
    if (interaction == Peripherial)
        lower = m_PeripherialLevel;
    unsigned upper = m_NumberOfKeys + 1;

    for (; lower < upper; ++lower)
    {
        StateNode::PeersList& cmds = *m_InputState->GetCommands(lower);
        for (StateNode* cmd : cmds)
        {
            Chord* chord = cmd->GetChord();
            if (chord->Item)
                continue;

            return chord;
        }
    }

    return nullptr;
}


Chord* Hand::Reserve(Chord* shortcut)
{
    unsigned level = shortcut->Keys.size();
    if (level > m_NumberOfKeys)
    {
        if (shortcut->Item)
            return nullptr;
        return shortcut;
    }

    StateNode::PeersList& cmds = *m_InputState->GetCommands(level);
    for (StateNode* cmd : cmds)
    {
        Chord* chord = cmd->GetChord();
        if (shortcut->IsValid(*chord))
        {
            if (chord->Item)
                return nullptr;
            return chord;
        }
    }

    return nullptr;
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
        m_Record.Keys.clear();
    if (m_InputState->Press(index))
        m_Record.Keys.push_back((Chord::Finger)index);

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
        m_Record.Keys.pop_back();
        m_InputState->Release(m_Keys.size());
        return true;
    }

    if (m_InputState->Release(index) && m_InputState->IsClean())
        m_Interaction->Execute(m_Record);

    return true;
}


int Hand::GetKeyIndex(int k)
{
    for (size_t i = 0; i < m_Keys.size(); ++i)
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
