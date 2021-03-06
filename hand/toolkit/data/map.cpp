#include "data/map.h"


namespace Hmi {


void Map::Add(const std::string& key, Item* child)
{
    Item* old = m_Value[key];
    if (old && (old->GetParent() == this))
        delete old;

    child->SetParent(this);
    m_Value[key] = child;
    NotifyChanged();
}


bool Map::Contains(Item* item)
{
    for (auto it = m_Value.begin(); it != m_Value.end(); ++it)
        if (it->second == item)
            return true;
    return false;
}


void Map::Remove(Item* child)
{
    if (child->GetParent() == this)
        delete child;

    for (auto it = m_Value.begin(); it != m_Value.end(); ++it)
    {
        if (it->second == child)
        {
            m_Value.erase(it);
            NotifyChanged();

            return;
        }
    }
}


Item* Map::GetChild(const std::string& name) const
{
    auto it = m_Value.find(name);
    if (it != m_Value.end())
        return it->second;

    return nullptr;
}


Item* Map::GetChild(unsigned pos)
{
    unsigned i = 0;
    for (auto it = m_Value.begin(); it != m_Value.end(); ++it, ++i)
        if (i == pos)
            return it->second;

    return nullptr;
}


void Map::Clear()
{
    for (auto item : m_Value)
    {
        if (item.second->GetParent() == this)
        {
            item.second->SetParent(nullptr);
            delete item.second;
        }
    }

    m_Value.clear();
    NotifyChanged();
}

}
