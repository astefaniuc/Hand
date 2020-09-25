#include "view/layer/masterlayer.h"
#include "view/layer/textlayer.h"
#include "graph/method.h"
#include "view/theme.h"


MasterLayer::MasterLayer()
{
    m_Exit = new Action<MasterLayer>("Exit", "Close interface", this, &MasterLayer::Exit);
}


void MasterLayer::Rebuild()
{
    Clear();

    if (GetLayout()->GetField(TITLE)->IsVisible())
        m_Title = Insert(TITLE, new TextLayer(m_Data->GetName()));

    if (GetLayout()->GetField(DESCRIPTION)->IsVisible())
        m_Description = Insert(DESCRIPTION, new TextLayer(m_Data->GetDescription()));

    Interface* in = static_cast<Interface*>(m_Data);

    m_View = in->GetView();
    if (m_View)
        Insert(VIEW, m_View);

    HmiItem* controls = in->GetControls();
    if (controls)
        m_Control = Insert(CONTROL, controls->GetLayer());
}


Drawer* MasterLayer::CreatetDrawer()
{
    return GetTheme()->GetViewDrawer();
}


void MasterLayer::Clear()
{
    delete m_Title;
    delete m_Description;

    m_Control = nullptr;
    m_Description = nullptr;
    m_Title = nullptr;
    m_View = nullptr;

    m_Sublayers.clear();
}
