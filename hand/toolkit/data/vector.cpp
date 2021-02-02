#include "data/vector.h"
#include "view/layers/button.h"
#include "view/layers/vector.h"
#include "view/layers/listview.h"


namespace Hmi {


Layer* List::CreateButtonView()
{
    return new Layers::Button();
}


void List::ConnectButton(Layer* view)
{
    Layers::ListView* lv = dynamic_cast<Layers::ListView*>(view->GetParentLayer());
    if (lv)
    {
        if (m_ButtonActivation)
        {
            RemoveActivationClient(m_ButtonActivation);
            delete m_ButtonActivation;
        }
        m_ButtonActivation = new CCallback<Layer>(lv, &Layer::SetContent);
        AddActivationClient(m_ButtonActivation);
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
