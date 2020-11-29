#include "view/layers/view.h"
#include "data/method.h"
#include "view/theme.h"


namespace Layers {


View::View()
{
    m_Exit = new Action<View>("Exit", "Close interface", this, &View::Exit);
    m_Exit->m_Chord = Chord::FullHand();
}


void View::Rebuild()
{
    Map::Rebuild();

    Interface* in = static_cast<Interface*>(m_Data);

    Insert(VIEW, in->GetView());

    Collection& controls = in->GetControls();
    controls.Attach(m_Exit);
    Insert(CONTROL, controls.GetExpandedView());
}


Drawer* View::CreatetDrawer()
{
    return GetTheme()->GetViewDrawer();
}

}
