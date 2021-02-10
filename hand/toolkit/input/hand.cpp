#include "input/hand.h"
#include "input/chord.h"
#include "input/device.h"
#include "input/inputstate.h"
#include "input/interaction.h"
#include "data/interface.h"
#include "view/layer.h"
#include "view/layers/interface.h"
#include "view/layers/text.h"
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


Hmi::Interface* Hand::GetInitScreen()
{
    Layouts::Placed::Map* handLayout = new Layouts::Placed::Map();
    handLayout->SetField(Finger[Chord::Thumb], { 0.21, 0.6, 0.0, 0.0 });
    handLayout->SetField(Finger[Chord::Pointer], { 0.3, 0.4, 0.0, 0.0 });
    handLayout->SetField(Finger[Chord::Middle], { 0.45, 0.39, 0.0, 0.0 });
    handLayout->SetField(Finger[Chord::Ring], { 0.6, 0.41, 0.0, 0.0 });
    handLayout->SetField(Finger[Chord::Little], { 0.7, 0.5, 0.0, 0.0 });

    Layers::Vector* handLayer = static_cast<Layers::Vector*>(m_KeysHmi->GetExpandedView());
    handLayer->SetLayout(handLayout);
    handLayer->SetExpandChildren(true);

    Layout* screenLayout = Layouts::CreateView();
    screenLayout->GetField(DESCRIPTION)->SetAlignment(VAlignment::Bottom, HAlignment::HCenter);
    screenLayout->GetField(CONTROL)->SetVisible(false);
    screenLayout->GetField(LAYER_CONTROLS)->SetVisible(false);

    Hmi::Interface* initScreen = new Hmi::Interface(
        "Keyboard Initialization",
        "Press 5 keys on the keyboard, to initialize a Hand device.");
    initScreen->GetView()->Attach(m_KeysHmi);

    initScreen->GetExpandedView()->GetListLayer()->SetLayout(screenLayout);

    return initScreen;
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


Chord* Hand::Assign(Hmi::Item* item, InteractionLevel interaction)
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
            if (!chord->Assign(item))
                continue;

            return chord;
        }
    }

    return nullptr;
}


Chord* Hand::Reserve(Chord* shortcut)
{
    unsigned level = shortcut->keys.size();
    if (level > m_NumberOfKeys)
        return shortcut;

    StateNode::PeersList& cmds = *m_InputState->GetCommands(level);
    for (StateNode* cmd : cmds)
    {
        Chord* chord = cmd->GetChord();
        if (shortcut->IsValid(*chord))
            return chord;
    }
    return shortcut;
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
        m_Interaction->Execute(m_Record);

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
