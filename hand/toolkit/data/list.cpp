#include "data/list.h"
#include "view/layers/list.h"


namespace Hmi {


void List::Add(Item* child)
{
    child->SetParent(this);
    m_Value.push_back(child);
}


void List::Remove(Item* child)
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


Item* List::GetChild(const std::string& name) const
{
    for (Item* item : m_Value)
        if (item->GetName() == name)
            return item;

    return nullptr;
}


void List::Clear()
{
    for (Item* item : m_Value)
        if (item->GetParent() == this)
            delete item;

    m_Value.clear();
}


Layer* List::CreateExpandedView()
{
    return new Layers::List();
}

}
