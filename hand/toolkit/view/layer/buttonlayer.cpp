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


void ButtonLayer::SetContent(HmiItem* a_data)
{
    Layer::SetContent(a_data);

    m_Name->SetData(a_data->GetName());
    m_Description->SetData(a_data->GetDescription());
    // TODO: m_Command
}
