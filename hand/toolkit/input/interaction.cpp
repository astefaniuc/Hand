#include "input/interaction.h"
#include "input/hand.h"
#include "data/method.h"
#include "view/layers/list.h"
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
    delete m_Hand;
    delete m_MoveControlUp;
}


void Control::Start()
{
    if (!m_Hand->Init())
        m_Stack[0]->Show(m_Hand->GetInitScreen());
}


void Control::Rebuild()
{
    Clear();
    if (!m_Stack.size())
        return;

    m_Stack.back()->Update();
    m_Stack.back()->GetInteractionGroups(this);

    Layers::List* interfaceCtrls =
        m_Stack.back()->GetLayerControls()->GetExpandedView()->GetListLayer();
    interfaceCtrls->Update();

    if (m_Stack.size() > 1)
        interfaceCtrls->Insert(m_MoveControlUp->GetButtonView());

    m_Stack.back()->Insert(LAYER_CONTROLS, interfaceCtrls);
    AddGroup(interfaceCtrls);

    if (!m_Focus)
        SetFocus(m_Groups[0]);
    m_Focus->Update();

    for (Group* child : m_Groups)
        if (child != m_Focus)
            child->Update();
}


void Control::Clear()
{
    auto copy = m_Groups;
    for (Group* child : copy)
        delete child;

    m_Focus = nullptr;
}


void Control::SetTarget(Layers::Interface* target)
{
    m_Stack.push_back(target);
    target->ExitListeners.Add(this, &Control::OnTargetExit);
    target->ShowListeners.Add(this, &Control::OnTargetShow);

    Rebuild();
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
            Rebuild();
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

    Rebuild();
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
        for (auto command : group->GetCommands())
            if (command->GetChord()->IsValid(chord))
                return command->GetChord()->Item->Activate();
}



Group::Group(Layers::List* layer, Control* parent)
    : m_Target(layer), m_Parent(parent)
{
    m_Target->Update();
    m_Target->ExitListeners.Add(this, &Group::OnTargetExit);
}


Group::~Group()
{
    delete m_Focus;
    if (m_Target)
    {
        Clear();
        m_Target->ExitListeners.Remove(this);
    }
    m_Parent->Remove(this);
}


void Group::Clear()
{
    auto copy = m_Commands;
    for (Command* child : copy)
        delete child;
    m_Update = true;
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
    if (m_Focus)
        level = Hand::Focus;

    std::vector<Layer*> commands;
    m_Target->GetActiveItems(commands);
    for (Layer* item : commands)
    {
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

    if (m_Focus)
        m_Focus->Update();
}


void Group::SetFocus()
{
    Layers::List* layerCmds = m_Target->GetLayerControls()->GetExpandedView()->GetListLayer();
    layerCmds->ExitListeners.Add(this, &Group::OnFocusExit);
    m_Focus = new Group(layerCmds, m_Parent);
}


void Group::RemoveFocus()
{
    delete m_Focus;
    m_Focus = nullptr;
}


void Group::OnTargetExit(Layer*)
{
    m_Target = nullptr;
    delete this;
}



Command::Command(Group* parent, Chord* chord)
    : m_Parent(parent), m_Chord(chord)
{
    m_Chord->Item->Update();
    m_Chord->Item->GetListLayer()->Insert(CONTROL, GetLayer());
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
    m_Parent->Remove(this);
}


Layer* Command::GetLayer()
{
    if (!m_Layer)
        m_Layer = m_Chord->CreateLayer(m_Parent->GetControl()->GetHand());
    return m_Layer;

}


void Command::OnTargetExit(Layer*)
{
    m_Chord->Item = nullptr;
    delete this;
}

}
