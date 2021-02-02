#include "input/interaction.h"
#include "input/hand.h"
#include "data/interface.h"
#include "data/list.h"
#include "view/layer.h"
#include "view/layers/list.h"
#include "view/layers/interface.h"
#include <algorithm>


namespace Interaction {


Control::Control(Hand* hand) : m_Hand(hand)
{
    m_Hand->SetInteraction(this);
    m_TargetExit = new CCallback<Control>(this, &Control::RemoveTargetCb);
}


Control::~Control()
{
    delete m_Hand;
    delete m_TargetExit;
}


void Control::Start()
{
    if (!m_Hand->Init())
    {
        // Show init screen
        Hmi::Item* initView = m_Hand->GetInitScreen();
        // TODO: built-in way to add/set the parent view
        dynamic_cast<Hmi::List*>(m_Stack[0]->GetContent()->GetInterface()->GetView())
            ->Attach(initView);
        SetTarget(dynamic_cast<Layers::Interface*>(initView->GetExpandedView()));
    }
}


void Control::Update()
{
    Clear();
    m_Stack.back()->SetInteractionControl(this);
}


void Control::Clear()
{
    for (Group* child : m_Groups)
    {
        child->SetControl(nullptr);
        delete child;
    }
    m_Groups.clear();
}


void Control::SetTarget(Layers::Interface* target)
{
    if (m_Stack.size())
        m_Stack.back()->RemoveInteractionControl();
    m_Stack.push_back(target);
    target->AddOnExit(m_TargetExit);

    Update();
}


void Control::RemoveTarget(Layers::Interface* target)
{
    auto it = std::find(m_Stack.rbegin(), m_Stack.rend(), target);
    if (it != m_Stack.rend())
    {
        (*it)->RemoveInteractionControl();
        (*it)->RemoveOnExit(m_TargetExit);
        m_Stack.erase((++it).base());

        Update();
    }
}


void Control::RemoveTargetCb(Hmi::Item* caller)
{
    Hmi::Item* target = caller->GetParentInterface();
    for (auto it = m_Stack.rbegin(); it != m_Stack.rend(); ++it)
    {
        if ((*it)->GetContent() == target)
        {
            (*it)->RemoveInteractionControl();
            (*it)->RemoveOnExit(m_TargetExit);
            m_Stack.erase((++it).base());
            return;
        }
    }
}


void Control::Add(Group* child)
{
    auto it = std::find(m_Groups.begin(), m_Groups.end(), child);
    if (it != m_Groups.end())
        return;

    m_Groups.push_back(child);
    child->SetControl(this);
    child->Update();
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
        m_Focus->GetTarget()->RemoveFocus();
    m_Focus = child;
    child->GetTarget()->SetFocus();
}


void Control::Execute(const Chord& chord)
{
    for (auto group : m_Groups)
        for (auto command : group->GetCommands())
            if (command->GetChord()->IsValid(chord))
                return command->GetChord()->GetItem()->Activate();
}



Group::Group(Layers::List* layer) : m_Layer(layer)
{
    m_Layer->SetInteraction(this);
}


Group::~Group()
{
    m_Layer->SetInteraction(nullptr);
    if (m_Parent)
        m_Parent->Remove(this);

    Clear();
}


void Group::Clear()
{
    for (Command* child : m_Commands)
    {
        child->RemoveParent();
        delete child;
    }
    m_Commands.clear();
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

    std::vector<Hmi::Item*> commands;
    m_Layer->GetActiveItems(commands);
    for (Hmi::Item* item : commands)
    {
        Chord* chord = GetControl()->GetHand()->Assign(item, item->GetShortcut());
        if (!chord)
            // No more free chords
            return;
        Add(new Command(this, chord));
    }
}



Command::Command(Group* parent, Chord* chord)
    : m_Parent(parent), m_Chord(chord)
{
}


Command::~Command()
{
    if (m_Parent)
        m_Parent->Remove(this);
    m_Chord->ClearItem();
}


Layer* Command::GetLayer()
{
    return m_Chord->GetLayer(m_Parent->GetControl()->GetHand());
}

}
