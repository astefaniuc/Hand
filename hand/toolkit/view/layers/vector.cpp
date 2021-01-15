#include "view/layers/vector.h"
#include "view/layers/button.h"
#include "data/list.h"
#include "view/theme.h"


namespace Layers {


Vector::~Vector()
{
    for (Layer* sub : m_Sublayers)
        sub->SetParent(nullptr);
}


Layer* Vector::GetFirstChild()
{
    m_CurrentChild = m_Sublayers.cbegin();
    if (m_CurrentChild != m_Sublayers.cend())
        return *m_CurrentChild;
    return nullptr;
}


Layer* Vector::GetNextChild()
{
    ++m_CurrentChild;
    if (m_CurrentChild != m_Sublayers.cend())
        return *m_CurrentChild;
    return nullptr;
}


Layer* Vector::GetChild(const std::string& name) const
{
    Hmi::List* listData = dynamic_cast<Hmi::List*>(GetContent());
    if (listData)
        for (unsigned i = 0; i < m_Sublayers.size(); ++i)
            if (listData->GetChild(i + m_StartPosition)->GetName() == name)
                return m_Sublayers[i];
    return nullptr;
}


void Vector::Rebuild()
{
    m_Sublayers.clear();

    Hmi::List* listData = dynamic_cast<Hmi::List*>(GetContent());
    if (listData)
    {
        unsigned count = listData->Size() - m_StartPosition;
        if (GetMaxItemsToShow() < count)
            count = GetMaxItemsToShow();

        for (unsigned i = 0; i < count; ++i)

            // Create the sub-objects
            AddLayer(listData->GetChild(i + m_StartPosition));
    }
    else
        AddLayer(GetContent());
}


void Vector::AddLayer(Hmi::Item* data)
{
    if (m_ExpandChildren)
        Insert(data->GetExpandedView());
    else
    {
        Insert(data->GetButtonView());
        data->ConnectButton(this);
    }
}


Layer* Vector::Insert(Layer* a_child)
{
    GetLayout()->GetField(a_child->GetContent()->GetName())->SetItem(a_child);
    m_Sublayers.push_back(a_child);
    a_child->SetParent(this);
    SetModified();
    return a_child;
}


void Vector::Remove(Layer* a_child)
{
    for (unsigned i = 0; i < m_Sublayers.size(); ++i)
    {
        if (m_Sublayers[i] == a_child)
        {
            m_Sublayers.erase(m_Sublayers.begin() + i);
            SetModified();
            return;
        }
    }
}


Drawer* Vector::CreatetDrawer()
{
    return GetTheme()->GetListDrawer();
}


void Vector::SetExpandChildren(bool expand)
{
    if (m_ExpandChildren != expand)
    {
        m_ExpandChildren = expand;
        SetModifiedContent();
    }
}

}
