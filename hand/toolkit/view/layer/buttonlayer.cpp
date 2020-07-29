#include "view/layer/buttonlayer.h"
#include "view/layer/textlayer.h"
#include "view/theme.h"
#include "graph/method.h"


ButtonLayer::ButtonLayer() : Layer()
{
    // TODO: make this configurable
    m_Name = new TextLayer();
    m_Description = new TextLayer();
    m_Command = new TextLayer();
}


Drawer* ButtonLayer::CreatetDrawer()
{
    return GetTheme()->GetButtonDrawer();
}


void ButtonLayer::Update()
{
    m_Name->SetData(m_Data->GetName());
    m_Description->SetData(m_Data->GetDescription());
    // TODO: m_Command
}
