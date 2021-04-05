#include "input/interaction.h"
#include "input/hand.h"
#include "data/method.h"
#include "view/layers/list.h"
#include "view/layers/hmi/chord.h"
#include "view/layers/hmi/interface.h"
#include <algorithm>


namespace Interaction {


Control::Control(Hand* hand) : m_Hand(hand)
{
    m_Hand->SetInteraction(this);
    m_MoveControlUp = new Hmi::Action<Control>(
        "Parent control", "Set the input control to the parent interface.",
        this, &Control::PopTargetCb);
}


Control::~Control()
{
    for (Layers::Interface* target : m_Stack)
    {
        target->ExitListeners.Remove(this);
        target->ShowListeners.Remove(this);
    }
    Clear();

    delete m_Hand;
    delete m_MoveControlUp;
}


void Control::Start()
{
    if (!m_Hand->Init())
        m_Stack[0]->Show(m_Hand->GetInitScreen());
}


void Control::Update()
{
    if (!m_IsModified)
        return;

    Clear();
    if (!m_Stack.size())
        return;

    m_Stack.back()->GetInteractionGroups(this);

    m_InterfaceCtrls = m_Stack.back()->GetLayerControls()->GetExpandedView()->GetListLayer();
    m_InterfaceCtrls->ExitListeners.Add(this, &Control::OnInterfaceCtrlsExit);

    if (m_Stack.size() > 1)
        m_InterfaceCtrls->Insert(m_MoveControlUp->GetButtonView());

    m_Stack.back()->Insert(LAYER_CONTROLS, m_InterfaceCtrls);
    AddGroup(m_InterfaceCtrls);

    if (!m_Focus)
        SetFocus(m_Groups[0]);
    m_Focus->Update();

    for (Group* child : m_Groups)
        if (child != m_Focus)
            child->Update();

    m_IsModified = false;
}


void Control::Clear()
{
    for (Group* child : m_Groups)
        delete child;
    m_Groups.clear();
    m_Focus = nullptr;
    delete m_InterfaceCtrls;
}


void Control::SetTarget(Layers::Interface* target)
{
    m_Stack.push_back(target);
    target->ExitListeners.Add(this, &Control::OnTargetExit);
    target->ShowListeners.Add(this, &Control::OnTargetShow);

    m_IsModified = true;
}


void Control::OnTargetShow(Layer* inserted)
{
    Layers::Interface* newTarget = dynamic_cast<Layers::Interface*>(inserted);
    if (newTarget)
        SetTarget(newTarget);
}


void Control::OnTargetExit(Layer* target)
{
    for (auto it = m_Stack.rbegin(); it != m_Stack.rend(); ++it)
    {
        if (*it == target)
        {
            m_Stack.erase((++it).base());
            m_IsModified = true;
            return;
        }
    }
}


void Control::PopTargetCb(Layers::Item*)
{
    Layers::Interface* target = m_Stack.back();
    target->ExitListeners.Remove(this);
    target->ShowListeners.Remove(this);
    m_Stack.pop_back();

    m_IsModified = true;
}


void Control::AddGroup(Layers::List* items, bool hasFocus)
{
    Interaction::Group* child = new Interaction::Group(items, this);
    m_Groups.push_back(child);
    if (hasFocus)
        SetFocus(child);
}


void Control::Remove(Group* child)
{
    auto it = std::find(m_Groups.begin(), m_Groups.end(), child);
    if (it != m_Groups.end())
        m_Groups.erase(it);
}


void Control::SetFocus(Group* child)
{
    if (m_Focus)
        m_Focus->RemoveFocus();
    m_Focus = child;
    child->SetFocus();
}


void Control::Execute(const Chord& chord)
{
    for (auto group : m_Groups)
        if (group->Execute(chord))
            return;
}



Group::Group(Layers::List* layer, Control* parent)
    : m_Target(layer), m_Parent(parent)
{
    m_Target->ExitListeners.Add(this, &Group::OnTargetExit);
    m_Target->DataListeners.Add(this, &Group::OnTargetDataChanged);
}


Group::~Group()
{
    if (m_Focus)
        RemoveFocus();
    if (m_Target)
    {
        Clear();
        m_Target->ExitListeners.Remove(this);
        m_Target->DataListeners.Remove(this);
    }
}


void Group::Clear()
{
    for (Command* child : m_Commands)
        delete child;
    m_Commands.clear();
}


void Group::Remove(Command* child)
{
    auto it = std::find(m_Commands.begin(), m_Commands.end(), child);
    if (it != m_Commands.end())
        m_Commands.erase(it);
}


void Group::Update()
{
    Clear();

    Hand::InteractionLevel level = Hand::Peripherial;
    if (m_HasFocus)
        level = Hand::Focus;

    std::vector<Layer*> commands;
    m_Target->GetActiveItems(commands);
    for (Layer* item : commands)
    {
        if (!item->GetListLayer())
            continue;

        Chord* chord = nullptr;
        Hmi::Item* data = item->GetData();
        if (data && data->GetShortcut())
            chord = GetControl()->GetHand()->Reserve(data->GetShortcut());
        else
            chord = GetControl()->GetHand()->GetFreeChord(level);

        if (!chord)
            // No more free chords
            return;

        chord->Item = item;
        Add(new Command(this, chord));
    }

    if (m_HasFocus)
    {
        if (!m_Focus)
            SetFocus();
        m_Focus->Update();
    }
}


void Group::SetFocus()
{
    m_HasFocus = true;

    Layers::List* layerCmds = m_Target->GetLayerControls()->GetExpandedView()->GetListLayer();
    layerCmds->ExitListeners.Add(this, &Group::OnFocusExit);
    m_Target->Insert(LAYER_CONTROLS, layerCmds);
    m_Focus = new Group(layerCmds, m_Parent);
}


void Group::RemoveFocus()
{
    delete m_Focus->GetTarget();
    delete m_Focus;
    m_Focus = nullptr;
    m_HasFocus = false;
}


bool Group::Execute(const Chord& chord)
{
    for (auto command : m_Commands)
    {
        if (command->GetChord()->IsValid(chord))
        {
            command->GetChord()->Item->Activate();
            return true;
        }
    }
    if (m_Focus)
        return m_Focus->Execute(chord);

    return false;
}


void Group::OnTargetExit(Layer*)
{
    m_Target = nullptr;
    m_Parent->Remove(this);
    delete this;
}



Command::Command(Group* parent, Chord* chord)
    : m_Parent(parent), m_Chord(chord)
{
    m_Layer = new Layers::Chord();
    m_Layer->Insert("", m_Chord->CreateLayer(m_Parent->GetControl()->GetHand()));
    m_Chord->Item->GetListLayer()->Insert(CHORD, m_Layer);
    m_Chord->Item->ExitListeners.Add(this, &Command::OnTargetExit);
}


Command::~Command()
{
    if (m_Chord->Item)
    {
        m_Chord->Item->ExitListeners.Remove(this);
        m_Chord->Item = nullptr;
        delete m_Layer;
    }
}


void Command::OnTargetExit(Layer*)
{
    m_Chord->Item = nullptr;
    m_Parent->Remove(this);
    delete this;
}

}
