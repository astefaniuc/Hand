#include "view/layers/map.h"


namespace Layers {


Map::~Map()
{
    for (auto entry : m_Sublayers)
        entry.second->SetParent(nullptr);
}


Layer* Map::GetFirstChild()
{
    m_CurrentChild = m_Sublayers.cbegin();
    if (m_CurrentChild != m_Sublayers.cend())
        return m_CurrentChild->second;
    return nullptr;
}


Layer* Map::GetNextChild()
{
    ++m_CurrentChild;
    if (m_CurrentChild != m_Sublayers.cend())
        return m_CurrentChild->second;
    return nullptr;
}


void Map::Rebuild()
{
    m_Sublayers.clear();

    m_Name.SetData(m_Data->GetName());
    m_Info.SetData(m_Data->GetDescription());

    Insert(TITLE, &m_Name);
    if (!m_Data->GetDescription().empty())
        Insert(DESCRIPTION, &m_Info);
}


void Map::Insert(const std::string& field, Layer* child)
{
    if (!child || !GetLayout()->GetField(field)->IsVisible())
        return;

    m_Sublayers[field] = child;
    child->SetParent(this);
    m_IsModified = true;
}


void Map::Remove(Layer* child)
{
    for (auto it = m_Sublayers.begin(); it != m_Sublayers.end(); ++it)
    {
        if (it->second == child)
        {
            m_Sublayers.erase(it);
            m_IsModified = true;
            return;
        }
    }
}


Layer* Map::GetChild(const std::string& name) const
{
    auto it = m_Sublayers.find(name);
    if (it != m_Sublayers.end())
        return it->second;
    return nullptr;
}

}
