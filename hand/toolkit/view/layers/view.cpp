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
    exit->m_Chord = Chord::FullHand();

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
    Layer::SetContent(data);
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


void View::UpdateFocus()
{
    m_Controls->SetFocus(m_Hand);
    if (!m_Controls->IsVisible())
        m_Controls->Update();

    Layer* layerCmds = m_LayerCommands->GetExpandedView();
    layerCmds->SetFocus(m_Hand);
    if (!layerCmds->IsVisible())
        layerCmds->Update();
}


void View::ClearFocus(  )
{
    m_Controls->ReleaseFocus(m_Hand);
    m_LayerCommands->GetExpandedView()->ReleaseFocus(m_Hand);
}


void View::Quit(Hmi::Item*)
{
    if (m_Hand)
    {
        m_Hand->ReleaseFocus(this);
        ReleaseFocus(m_Hand);
    }
    Map::Quit(nullptr);
}


Drawer* View::CreatetDrawer() { return GetTheme()->GetViewDrawer(); }
Layout* View::CreateLayout() { return Layouts::CreateView(); }

}
