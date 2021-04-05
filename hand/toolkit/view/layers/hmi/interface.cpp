#include "view/layers/hmi/interface.h"
#include "view/layers/hmi/listview.h"
#include "view/layers/text.h"
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
    SetExpandChildren(true);
}


void Interface::Rebuild()
{
    Insert(TITLE, new RawText(m_Data->GetName()));
    if (!m_Data->GetInfo().empty())
        Insert(DESCRIPTION, new RawText(m_Data->GetInfo()));


    m_Controls = new ListView();
    m_Controls->SetData(m_Data->GetInterface()->GetControls());
    Insert(CONTROLS, m_Controls);

    Vector* view = new Vector();
    view->SetExpandChildren(true);
    view->SetData(m_Data->GetInterface()->GetView());
    Insert(VIEW, view);
}


void Interface::Show(Layer* item)
{
    GetView()->Insert(item);
    ShowListeners.Notify(item);
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


void Interface::GetInteractionGroups(Interaction::Control* control)
{
    control->AddGroup(m_Controls, true);
    control->AddGroup(GetView());

    // CollectShortcuts();
    // if (m_Shortcuts->Size())
    //     control->AddGroup(m_Shortcuts->GetExpandedView()->GetListLayer());
}


Drawer* Interface::GetDrawerFromTheme() { return GetTheme()->GetViewDrawer(this); }
Layout* Interface::CreateLayout() { return Layouts::CreateView(); }

}
