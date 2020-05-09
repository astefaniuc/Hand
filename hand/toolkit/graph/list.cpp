#include "graph/list.h"


void List::Add(HmiItem* a_child)
{
    a_child->SetParent(this);
    m_Value.push_back(a_child);
}


void List::Remove(HmiItem* a_child)
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


HmiItem* List::GetChild(const std::string& a_name)
{
    for (HmiItem* child : m_Value)
        if (child->GetName() == a_name)
            return child;
    return nullptr;
}


void List::Clear()
{
    for (HmiItem* child : m_Value)
        if (child->GetParent() == this)
            delete child;
    m_Value.clear();
}
