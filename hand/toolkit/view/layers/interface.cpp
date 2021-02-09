#include "view/layers/interface.h"
#include "view/layers/listview.h"
#include "data/method.h"
#include "data/vector.h"
#include "view/theme.h"
#include "input/hand.h"


namespace Layers {


Interface::Interface()
{
    Hmi::Action<Interface>* exit = new Hmi::Action<Interface>(EXIT, "Close interface", this, &Interface::Quit);
    exit->SetShortcut(Chord::FullHand());

    GetLayerControls()->Add(exit);

    m_Controls = new ListView();
}


Interface::~Interface()
{
    delete m_Controls;
}


void Interface::SetData(Hmi::Item* data)
{
    m_Controls->SetData(data->GetInterface()->GetControls());
    Map::SetData(data);
}


void Interface::Update()
{
    // Remove view items in the next iteration.
    for (Interface* layer : m_ToRemoveFromView)
        GetView()->Remove(layer->GetData());
    m_ToRemoveFromView.clear();

    Map::Update();
}


void Interface::Rebuild()
{
    Map::Rebuild();

    Hmi::Item* view = m_Data->GetInterface()->GetView();

    if (view)
        Insert(VIEW, view->GetExpandedView());
    Insert(CONTROL, m_Controls);
    Insert(LAYER_CONTROLS, GetLayerControls()->GetExpandedView());
}


void Interface::GetActiveItems(std::vector<Hmi::Item*>& out)
{

    Hmi::Item* view = m_Data->GetInterface()->GetView();

    out.push_back(m_Controls->GetData());
    if (view)
        out.push_back(view);
    out.push_back(GetLayerControls());
}


void Interface::AddView(Hmi::Item* item)
{
    GetView()->Add(item);
    SetViewRemoveCallback(item);
}


void Interface::AttachView(Hmi::Item* item)
{
    GetView()->Add(item);
    SetViewRemoveCallback(item);
}


void Interface::SetViewRemoveCallback(Hmi::Item* item)
{
    item->GetExpandedView()->GetInterface()->ExitListeners.Add(
        this, &Interface::AddToRemoveFromView);
}


void Interface::CollectShortcuts()
{
    if (!m_Shortcuts)
        m_Shortcuts = new Hmi::Vector("Shortcuts", "");
    else
        m_Shortcuts->Clear();

    GetLayerControls()->GetShortcuts(nullptr, m_Shortcuts);
    if (GetData())
        GetData()->GetShortcuts(GetData()->GetInterface(), m_Shortcuts);
}


void Interface::SetInteractionControl(Interaction::Control* control)
{
    if (m_InteractionControl)
        return;

    m_InteractionControl = control;

    Interaction::Group* ctrlGroup = new Interaction::Group(m_Controls);
    control->Add(ctrlGroup);
    control->SetFocus(ctrlGroup);

    control->Add(new Interaction::Group(this));

    CollectShortcuts();
    if (m_Shortcuts->Size())
        control->SetShortcuts(new Interaction::Group(
            m_Shortcuts->GetExpandedView()->GetListLayer()));

    SetModified();
}


void Interface::RemoveInteractionControl()
{
    m_InteractionControl = nullptr;
    SetModified();
}


void Interface::Quit(Hmi::Item* caller)
{
    Map::Quit(nullptr);

    ExitListeners.Execute(this);
}


Drawer* Interface::CreatetDrawer() { return GetTheme()->GetViewDrawer(); }
Layout* Interface::CreateLayout() { return Layouts::CreateView(); }

}
