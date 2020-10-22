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


Layer* Map::Insert(const std::string& field, Layer* child)
{
    m_Sublayers[field] = child;
    child->SetParent(this);
    m_IsModified = true;
    return child;
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
