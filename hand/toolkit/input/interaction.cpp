#include "input/interaction.h"
#include "input/hand.h"
#include "data/interface.h"
#include "data/list.h"
#include "data/method.h"
#include "view/layer.h"
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

    if (m_Stack.size() > 1)
        m_Stack.back()->GetLayerControls()->Attach(m_MoveControlUp);
    m_Stack.back()->SetInteractionControl(this);

    if (!m_Focus)
    {
        if (!m_Groups.size())
            return PopTargetCb(nullptr);

        m_Focus = m_Groups[0];
    }

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
    if (m_Stack.size())
        m_Stack.back()->RemoveInteractionControl();
    m_Stack.push_back(target);
    target->ExitListeners.Add(this, &Control::RemoveTargetCb);

    Rebuild();
}


void Control::RemoveTarget(Layers::Interface* target)
{
    auto it = std::find(m_Stack.rbegin(), m_Stack.rend(), target);
    if (it != m_Stack.rend())
        RemoveTarget(it);
}


void Control::RemoveTargetCb(Layers::Interface* target)
{
    for (auto it = m_Stack.rbegin(); it != m_Stack.rend(); ++it)
    {
        if (*it == target)
        {
            RemoveTarget(it);
            return;
        }
    }
}


void Control::PopTargetCb(Hmi::Item*)
{
    auto it = m_Stack.rbegin();
    RemoveTarget(it);
}


void Control::RemoveTarget(std::vector<Layers::Interface*>::reverse_iterator& target)
{
    (*target)->RemoveInteractionControl();
    (*target)->GetLayerControls()->Remove(m_MoveControlUp);
    (*target)->ExitListeners.Remove(this);
    m_Stack.erase((++target).base());

    Rebuild();
}


void Control::AddGroup(Layers::List* items, bool hasFocus)
{
    Interaction::Group* child = new Interaction::Group(items);
    m_Groups.push_back(child);
    child->SetControl(this);
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
                return command->GetChord()->GetItem()->GetData()->Activate();
}



Group::Group(Layers::List* layer) : m_Target(layer)
{
    m_Target->Update();
    m_Target->SetInteraction(this);
}


Group::~Group()
{
    Clear();
    m_Target->SetInteraction(nullptr);
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


void Group::Release()
{
    Clear();
}


void Group::Update()
{
    Clear();

    Hand::InteractionLevel level = Hand::Peripherial;
    if (HasFocus())
        level = Hand::Focus;

    std::vector<Layer*> commands;
    m_Target->GetActiveItems(commands);
    for (Layer* item : commands)
    {
        if (item->GetCommand())
            continue;

        Chord* chord = nullptr;
        Hmi::Item* data = item->GetData();
        if (data && data->GetShortcut())
        {
            chord = GetControl()->GetHand()->Reserve(data->GetShortcut());
            chord->Assign(item);
        }
        else
            chord = GetControl()->GetHand()->Assign(item, level);

        if (!chord)
            // No more free chords
            return;

        Add(new Command(this, chord));
    }
}


void Group::SetFocus()
{
    m_HasFocus = true;
    GetControl()->AddGroup(GetTarget()->GetLayerControls()->GetExpandedView()->GetListLayer());
}


void Group::RemoveFocus()
{
    m_HasFocus = false;
}



Command::Command(Group* parent, Chord* chord)
    : m_Parent(parent), m_Chord(chord)
{
    m_Chord->GetItem()->SetCommand(this);
}


Command::~Command()
{
    m_Chord->GetItem()->ReleaseCommand();
    m_Chord->ClearItem();
    m_Parent->Remove(this);
    delete m_Layer;
}


Layer* Command::GetLayer()
{
    if (!m_Layer)
        m_Layer = m_Chord->CreateLayer(m_Parent->GetControl()->GetHand());
    return m_Layer;

}

}
