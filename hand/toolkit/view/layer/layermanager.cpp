#include "view/layer/layermanager.h"
#include "input/device.h"
#include "input/inputstate.h"
#include "graph/method.h"
#include "view/theme.h"


MasterLayer::MasterLayer()
{
    m_Exit = new Action<MasterLayer>("Exit", "Close interface", this, &MasterLayer::Exit);
}


void MasterLayer::SetContent(HmiItem* a_hmi)
{
    Clear();

    Interface* in = dynamic_cast<Interface*>(a_hmi);
    m_View = Insert(in->GetView());
    HmiItem* controls = in->GetControls();
    if (controls)
        m_Controls = Insert(controls->GetLayer());

    HmiItem* aux = in->GetAuxilliary();
    if (aux)
        m_Auxilliary = Insert(aux->GetLayer());
}


Drawer* MasterLayer::CreatetDrawer()
{
    return GetTheme()->GetButtonDrawer();
}


void MasterLayer::Clear()
{
    m_View = nullptr;
    m_Controls = nullptr;
    m_Auxilliary = nullptr;
}

void MasterLayer::Exit(HmiItem*)
{
    // Suicide
    delete(this);
}
