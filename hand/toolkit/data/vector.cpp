#include "data/vector.h"
#include "view/layers/hmi/button.h"
#include "view/layers/vector.h"
#include "view/layers/hmi/listview.h"


namespace Hmi {


void List::GetShortcuts(Hmi::Interface* caller, Hmi::List* out)
{
    Item::GetShortcuts(caller, out);
    for (unsigned i = 0; i < Size(); ++i)
        GetChild(i)->GetShortcuts(caller, out);
}


void List::Execute(Layers::Item* caller)
{
    Layers::ListView* lv = dynamic_cast<Layers::ListView*>(
        caller->GetParentLayer()->GetParentLayer());
    if (lv)
        lv->SetData(this);
}


Layer* List::CreateExpandedData()
{
    return new Layers::Vector();
}



void Vector::Add(Item* child)
{
    child->SetParent(this);
    m_Value.push_back(child);
    NotifyChanged();
}


void Vector::Attach(Item* child)
{
    m_Value.push_back(child);
    NotifyChanged();
}


bool Vector::Contains(Item* item)
{
    for (Item* child : m_Value)
        if (child == item)
            return true;
    return false;
}


void Vector::Remove(Item* child)
{
    if (child->GetParent() == this)
        delete child;

    for (size_t i = 0; i < m_Value.size(); ++i)
    {
        if (m_Value[i] == child)
        {
            m_Value.erase(m_Value.begin() + i);
            NotifyChanged();
            return;
        }
    }
}


Item* Vector::GetChild(const std::string& name) const
{
    for (Item* item : m_Value)
        if (item->GetName() == name)
            return item;

    return nullptr;
}


void Vector::Clear()
{
    for (Item* item : m_Value)
    {
        if (item->GetParent() == this)
        {
            item->SetParent(nullptr);
            delete item;
        }
    }

    m_Value.clear();
    NotifyChanged();
}

}
