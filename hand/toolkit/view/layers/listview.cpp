#include "view/layers/listview.h"
#include "view/layouts/builtin.h"


namespace Layers {


ListView::ListView()
{
    m_LayerCommands = new Hmi::Vector(LAYER_CONTROLS, "");
}


void ListView::SetContent(Hmi::Item* data)
{
    Hand* tmp = m_Hand;
    if (tmp && m_ViewStack.size())
        ReleaseFocus(tmp);

    m_ViewStack.push_back(data);
    Layer::SetContent(data);
    Update();

    if (tmp)
        SetFocus(tmp);
}


void ListView::Rebuild()
{
    Map::Rebuild();

    Hmi::List* in = static_cast<Hmi::List*>(m_Data);

    Insert(VIEW, in->GetExpandedView());
    Insert(LAYER_CONTROLS, m_LayerCommands->GetExpandedView());
}


void ListView::SetFocus(Hand* hand)
{
    m_Hand = hand;
    Layer* ctrls = GetChild(VIEW);
    if (ctrls)
        ctrls->SetFocus(hand);
    ctrls = GetChild(LAYER_CONTROLS);
    if (ctrls)
        ctrls->SetFocus(hand);
}


void ListView::ReleaseFocus(Hand* hand)
{
    Layer* ctrls = GetChild(VIEW);
    if (ctrls)
        ctrls->ReleaseFocus(hand);
    ctrls = GetChild(LAYER_CONTROLS);
    if (ctrls)
        ctrls->ReleaseFocus(hand);
    m_Hand = nullptr;
}


Drawer* ListView::CreatetDrawer() { return GetTheme()->GetViewDrawer(); }
Layout* ListView::CreateLayout() { return Layouts::CreateListView(); }

}
