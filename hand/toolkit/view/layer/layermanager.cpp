#include "view/layer/layermanager.h"
#include "input/device.h"
#include "input/inputstate.h"
#include "graph/method.h"


MasterLayer::MasterLayer()
{
    BufferType = COLLECTOR;

    m_Exit = new Action<MasterLayer>("Exit", "Close interface", this, &MasterLayer::Exit);
}


void MasterLayer::Show(Interface* a_hmi)
{
    Clear();

    m_View = Insert(a_hmi->GetView());
    HmiItem* controls = a_hmi->GetControls();
    if (controls)
        m_Controls = Insert(controls->GetLayer());

    HmiItem* aux = a_hmi->GetAuxilliary();
    if (aux)
        m_Auxilliary = Insert(aux->GetLayer());
}


void MasterLayer::Exit(HmiItem*)
{
    // Suicide
    delete(this);
}
