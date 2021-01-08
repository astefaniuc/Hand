#include "view/layers/view.h"
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
}


void View::Rebuild()
{
    Map::Rebuild();

    Hmi::Interface* in = static_cast<Hmi::Interface*>(m_Data);

    Insert(VIEW, in->GetView()->GetExpandedView());
    Insert(CONTROL, in->GetControls()->GetExpandedView());
    Insert(LAYER_CONTROLS, m_LayerCommands->GetExpandedView());
}


void View::SetFocus(Hand* hand)
{
    m_Hand = hand;
    Layer* ctrls = GetChild(CONTROL);
    if (ctrls)
        ctrls->SetFocus(hand);
    ctrls = GetChild(LAYER_CONTROLS);
    if (ctrls)
        ctrls->SetFocus(hand);
}


void View::ReleaseFocus(Hand* hand)
{
    Layer* ctrls = GetChild(CONTROL);
    if (ctrls)
        ctrls->ReleaseFocus(hand);
    ctrls = GetChild(LAYER_CONTROLS);
    if (ctrls)
        ctrls->ReleaseFocus(hand);
    m_Hand = nullptr;
}


void View::Quit(Hmi::Item*)
{
    if (m_Hand)
    {
        m_Hand->ReleaseFocus(this);
        m_Hand = nullptr;
    }
    Map::Quit(nullptr);
}


Drawer* View::CreatetDrawer() { return GetTheme()->GetViewDrawer(); }
Layout* View::CreateLayout() { return Layouts::CreateView(); }

}
