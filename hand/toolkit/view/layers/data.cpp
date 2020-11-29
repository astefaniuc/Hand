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
    Map::Rebuild();

    m_DataText.SetData(static_cast<Data*>(m_Data)->GetValueString());
    Insert(VIEW, &m_DataText);
}


Drawer* DataLayer::CreatetDrawer()
{
    return GetTheme()->GetDataDrawer();
}

}
