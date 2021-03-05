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
}


void Interface::Rebuild()
{
    Item::Rebuild();

    m_Controls = new ListView();
    m_Controls->SetData(m_Data->GetInterface()->GetControls());
    Insert(CONTROL, m_Controls);

    if (m_Data->GetInterface()->GetView())
    {
        m_View = new Vector();
        m_View->SetExpandChildren(true);
        m_View->SetData(m_Data->GetInterface()->GetView());
        Insert(VIEW, m_View);
    }
}


void Interface::Show(Layer* item)
{
    m_View->Insert(item);
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
    Update();

    control->AddGroup(m_Controls, true);
    control->AddGroup(m_View);

    // CollectShortcuts();
    // if (m_Shortcuts->Size())
    //     control->AddGroup(m_Shortcuts->GetExpandedView()->GetListLayer());
}


Drawer* Interface::GetDrawerFromTheme() { return GetTheme()->GetViewDrawer(this); }
Layout* Interface::CreateLayout() { return Layouts::CreateView(); }

}
