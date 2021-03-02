#include "view/layers/hmi/interface.h"
#include "view/layers/hmi/listview.h"
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
    Item::SetData(data);
}


void Interface::Rebuild()
{
    Item::Rebuild();

    Insert(VIEW, m_View);
    Insert(CONTROL, m_Controls);
    Insert(LAYER_CONTROLS, GetLayerControls()->GetExpandedView());
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

    control->AddGroup(m_Controls, true);
    control->AddGroup(this);
    control->AddGroup(GetLayerControls()->GetExpandedView()->GetListLayer());

    CollectShortcuts();
    if (m_Shortcuts->Size())
        control->AddGroup(m_Shortcuts->GetExpandedView()->GetListLayer());

    SetModified();
}


void Interface::RemoveInteractionControl()
{
    m_InteractionControl = nullptr;
    SetModified();
}


Drawer* Interface::GetDrawerFromTheme() { return GetTheme()->GetViewDrawer(this); }
Layout* Interface::CreateLayout() { return Layouts::CreateView(); }

}
