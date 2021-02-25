#include "view/layers/interface.h"
#include "view/layers/listview.h"
#include "view/layers/vector.h"
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
    m_View = new Vector();
    m_View->SetExpandChildren(true);
}


Interface::~Interface()
{
    delete m_Controls;
}


void Interface::SetData(Hmi::Item* data)
{
    m_Controls->SetData(data->GetInterface()->GetControls());
    if (data->GetInterface()->GetView())
        m_View->SetData(data->GetInterface()->GetView());
    m_View->Update();
    Map::SetData(data);
}


void Interface::Rebuild()
{
    Map::Rebuild();

    Insert(VIEW, m_View);
    Insert(CONTROL, m_Controls);
    Insert(LAYER_CONTROLS, GetLayerControls()->GetExpandedView());
}


void Interface::GetActiveItems(std::vector<Hmi::Item*>& out)
{
    out.push_back(m_Controls->GetData());
    out.push_back(GetView());
    out.push_back(GetLayerControls());
}


void Interface::Show(Layer* item)
{
    m_View->Insert(item);
    if (m_InteractionControl)
    {
        Layers::Interface* i = item->GetInterface();
        if (i && (i == item))
            m_InteractionControl->SetTarget(i);
    }
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

    control->Add(new Interaction::Group(GetLayerControls()->GetExpandedView()->GetListLayer()));

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


Drawer* Interface::GetDrawerFromTheme() { return GetTheme()->GetViewDrawer(this); }
Layout* Interface::CreateLayout() { return Layouts::CreateView(); }

}
