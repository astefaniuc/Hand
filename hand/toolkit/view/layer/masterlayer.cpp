#include "view/layer/masterlayer.h"
#include "input/device.h"
#include "input/inputstate.h"
#include "graph/method.h"
#include "view/theme.h"


MasterLayer::MasterLayer()
{
    m_Exit = new Action<MasterLayer>("Exit", "Close interface", this, &MasterLayer::Exit);
}


void MasterLayer::Rebuild()
{
    Clear();

    if (GetLayout()->ShowName())
        m_Title = AddField(new TextLayer(m_Data->GetName()), TITLE);

    if (GetLayout()->ShowDescription())
        m_Description = AddField(new TextLayer(m_Data->GetDescription()), DESCRIPTION);

    Interface* in = dynamic_cast<Interface*>(m_Data);

    m_View = in->GetView();
    if (m_View)
        AddField(m_View, VIEW);

    HmiItem* controls = in->GetControls();
    if (controls)
        m_Control = AddField(controls->GetLayer(), CONTROL);
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

void MasterLayer::Exit(HmiItem*)
{
    // Suicide
    delete this;
}


Layer* MasterLayer::AddField(Layer* sub, const std::string& field)
{
    Insert(sub);
    sub->SetSize(Multiply(
        dynamic_cast<ViewLayout*>(GetLayout())->GetField(field),
        GetDrawer()->GetContentSize(GetSize())));
    return sub;
}
