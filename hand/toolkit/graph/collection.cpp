#include "graph/collection.h"
#include "view/layer/listlayer.h"


void Collection::Add(HmiItem* a_child)
{
    a_child->SetParent(this);
    m_Value.push_back(a_child);
}


void Collection::Remove(HmiItem* a_child)
{
    if (a_child->GetParent() == this)
        delete a_child;

    for (unsigned i = 0; i < m_Value.size(); ++i)
    {
        if (m_Value[i] == a_child)
        {
            m_Value.erase(m_Value.begin() + 1);
            return;
        }
    }
}


HmiItem* Collection::GetChild(const std::string& a_name)
{
    for (HmiItem* item : m_Value)
        if (item->GetName() == a_name)
            return item;

    return nullptr;
}


void Collection::Clear()
{
    for (HmiItem* item : m_Value)
        if (item->GetParent() == this)
            delete item;

    m_Value.clear();
}


Layer* Collection::CreateLayer()
{
    return new ListLayer();
}
