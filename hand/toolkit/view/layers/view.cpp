#include "view/layers/view.h"
#include "data/interface.h"
#include "data/method.h"
#include "data/vector.h"
#include "view/theme.h"


namespace Layers {


View::View()
{
    Hmi::Action<View>* exit = new Hmi::Action<View>("Exit", "Close interface", this, &View::Exit);
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


Drawer* View::CreatetDrawer() { return GetTheme()->GetViewDrawer(); }
Layout* View::CreateLayout() { return Layouts::CreateView(); }

}
