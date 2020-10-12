#include "view/layers/data.h"
#include "view/layers/text.h"
#include "view/theme.h"


namespace Layers {


void DataLayer::SetContent(HmiItem* data)
{
    Layer::SetContent(data);
    ((Data*)data)->AddDataChangedClient(
        new CCallback<DataLayer>(this, &DataLayer::NotifyChanged));
}


void DataLayer::Rebuild()
{
    Clear();

    m_Name = Insert(TITLE, new Text(m_Data->GetName() + ":"));
    if (!m_Data->GetDescription().empty())
        m_Description = Insert(DESCRIPTION, new Text(m_Data->GetDescription()));
    m_DataText = Insert(VIEW, new Text(static_cast<Data*>(m_Data)->GetValueString()));
    // TODO: m_Command
}


void DataLayer::Clear()
{
    delete m_Name;
    delete m_Description;
    delete m_DataText;

    m_DataText = nullptr;
    m_Description = nullptr;
    m_Name = nullptr;

    m_Sublayers.clear();
}


Drawer* DataLayer::CreatetDrawer()
{
    return GetTheme()->GetDataDrawer();
}

}
