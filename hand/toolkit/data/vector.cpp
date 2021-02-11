#include "data/vector.h"
#include "view/layers/button.h"
#include "view/layers/vector.h"
#include "view/layers/listview.h"


namespace Hmi {


void List::GetShortcuts(Hmi::Interface* caller, Hmi::List* out)
{
    Item::GetShortcuts(caller, out);
    for (auto i = 0; i < Size(); ++i)
        GetChild(i)->GetShortcuts(caller, out);
}


Layer* List::CreateButtonView()
{
    return new Layers::Button();
}


void List::Expand(Item*)
{
    if (!GetExpandedView()->IsVisible())
    {
        Layers::ListView* lv = dynamic_cast<Layers::ListView*>(
            GetButtonView()->GetParentLayer()->GetParentLayer());
        if (lv)
            lv->SetData(this);
    }
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


void Vector::Remove(Item* child)
{
    if (child->GetParent() == this)
        delete child;

    for (unsigned i = 0; i < m_Value.size(); ++i)
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


Layer* Vector::CreateExpandedView()
{
    return new Layers::Vector();
}

}
