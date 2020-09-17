#include "view/layer/buttonlayer.h"
#include "view/layer/textlayer.h"
#include "view/theme.h"


void DataLayer::SetContent(HmiItem* data)
{
    Layer::SetContent(data);
    ((Data*)data)->AddDataChangedClient(
        new CCallback<DataLayer>(this, &DataLayer::NotifyChanged));
}


void DataLayer::Rebuild()
{
    Clear();

    m_Name = Insert(new TextLayer(m_Data->GetName() + ":"));
    m_Description = Insert(new TextLayer(m_Data->GetDescription()));
    m_DataText = Insert(new TextLayer(dynamic_cast<Data*>(m_Data)->GetValueString()));
    // TODO: m_Command
}


void DataLayer::UpdateSubSizes()
{
    SetSubSize(m_Name, TITLE);
    SetSubSize(m_Description, DESCRIPTION);
    SetSubSize(m_DataText, VIEW);
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


void ButtonLayer::Rebuild()
{
    Clear();

    m_Name = Insert(new TextLayer(m_Data->GetName()));
    m_Description = Insert(new TextLayer(m_Data->GetDescription()));
    // TODO: m_Control
}


void ButtonLayer::UpdateSubSizes()
{
    SetSubSize(m_Name, TITLE);
    SetSubSize(m_Description, DESCRIPTION);
}


void ButtonLayer::Clear()
{
    delete m_Name;
    delete m_Description;

    m_Description = nullptr;
    m_Name = nullptr;

    m_Sublayers.clear();
}


Drawer* ButtonLayer::CreatetDrawer()
{
    return GetTheme()->GetButtonDrawer();
}
