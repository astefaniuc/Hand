#include "view/layers/map.h"


namespace Layers {


void Map::Rebuild()
{
    m_Name.SetData(m_Data->GetName());
    m_Info.SetData(m_Data->GetInfo());

    Insert(TITLE, &m_Name);
    if (!m_Data->GetInfo().empty())
        Insert(DESCRIPTION, &m_Info);
    if (m_Control)
        Insert(CONTROL, m_Control);
}


void Map::Insert(const std::string& name, Layer* child)
{
    if (!child)
        return;

    Field* field = GetLayout()->GetField(name, false);
    if (!field)
        return;

    field->SetItem(child);
    SetModified();
}


void Map::RemoveControl()
{
    m_Control->Exit();
    delete m_Control;
    m_Control = nullptr;
}

}
