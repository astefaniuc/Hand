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

    if (GetLayout()->GetShowName())
        m_Title = Insert(new TextLayer(m_Data->GetName()));

    if (GetLayout()->GetShowDescription())
        m_Description = Insert(new TextLayer(m_Data->GetDescription()));

    Interface* in = dynamic_cast<Interface*>(m_Data);

    m_View = in->GetView();
    if (m_View)
        Insert(m_View);

    HmiItem* controls = in->GetControls();
    if (controls)
        m_Control = Insert(controls->GetLayer());
}


void MasterLayer::UpdateSubSizes()
{
    SetSubSize(m_Title, TITLE);
    SetSubSize(m_Description, DESCRIPTION);
    SetSubSize(m_View, VIEW);
    SetSubSize(m_Control, CONTROL);
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
