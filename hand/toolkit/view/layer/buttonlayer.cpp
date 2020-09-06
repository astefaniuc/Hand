#include "view/layer/buttonlayer.h"
#include "view/layer/textlayer.h"
#include "view/theme.h"
#include "graph/method.h"


void DataLayer::Rebuild()
{
    Clear();

    m_Name = new TextLayer();
    AddField(m_Name, TITLE);
    m_Description = new TextLayer();
    AddField(m_Description, DESCRIPTION);
    m_DataText = new TextLayer();
    AddField(m_DataText, VIEW);

    m_Name->SetData(m_Data->GetName() + ":");
    m_Description->SetData(m_Data->GetDescription());
    m_DataText->SetData(dynamic_cast<Data*>(m_Data)->GetValueString());
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


void ButtonLayer::Rebuild()
{
    Clear();

    m_Name = new TextLayer();
    AddField(m_Name, TITLE);
    m_Description = new TextLayer();
    AddField(m_Description, DESCRIPTION);
    // TODO: external layer
    // AddField(m_Control, CONTROL);

    m_Name->SetData(m_Data->GetName());
    m_Description->SetData(m_Data->GetDescription());
    // TODO: m_Control
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
