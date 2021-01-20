#include "view/layers/view.h"
#include "view/layers/listview.h"
#include "data/interface.h"
#include "data/method.h"
#include "data/vector.h"
#include "view/theme.h"
#include "input/hand.h"


namespace Layers {


View::View()
{
    Hmi::Action<View>* exit = new Hmi::Action<View>(EXIT, "Close interface", this, &View::Quit);
    exit->SetShortcut(Chord::FullHand());

    m_LayerCommands = new Hmi::Vector(LAYER_CONTROLS, "");
    m_LayerCommands->Add(exit);

    m_Controls = new ListView();
}


View::~View()
{
//    delete m_LayerCommands;
    delete m_Controls;
}


void View::AddOnExit(ICallback* cb)
{
    m_LayerCommands->GetChild(EXIT)->AddActivationClient(cb);
}


void View::SetContent(Hmi::Item* data)
{
    m_Controls->SetContent(static_cast<Hmi::Interface*>(data)->GetControls());
    Map::SetContent(data);
}


void View::Rebuild()
{
    Map::Rebuild();

    Hmi::Interface* in = static_cast<Hmi::Interface*>(m_Data);

    if (in->GetView())
        Insert(VIEW, in->GetView()->GetExpandedView());
    Insert(CONTROL, m_Controls);
    Insert(LAYER_CONTROLS, m_LayerCommands->GetExpandedView());
}


bool View::UpdateFocus()
{
    m_Controls->SetFocus(m_Hand);
    m_LayerCommands->GetExpandedView()->SetFocus(m_Hand);

    return false;
}


void View::ClearFocus()
{
    m_Controls->ReleaseFocus(m_Hand);
    m_LayerCommands->GetExpandedView()->ReleaseFocus(m_Hand);
}


void View::Quit(Hmi::Item*)
{
    if (m_Hand)
        m_Hand->ReleaseFocus(this);

    Map::Quit(nullptr);
}


Drawer* View::CreatetDrawer() { return GetTheme()->GetViewDrawer(); }
Layout* View::CreateLayout() { return Layouts::CreateView(); }

}
