#include "data/collection.h"
#include "view/layers/list.h"


void Collection::Add(HmiItem* child)
{
    child->SetParent(this);
    m_Value.push_back(child);
}


void Collection::Remove(HmiItem* child)
{
    if (child->GetParent() == this)
        delete child;

    for (unsigned i = 0; i < m_Value.size(); ++i)
    {
        if (m_Value[i] == child)
        {
            m_Value.erase(m_Value.begin() + 1);
            return;
        }
    }
}


HmiItem* Collection::GetChild(const std::string& name) const
{
    for (HmiItem* item : m_Value)
        if (item->GetName() == name)
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


Layer* Collection::CreateExpandedView()
{
    return new Layers::List();
}
