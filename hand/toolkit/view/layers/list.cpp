#include "view/layers/list.h"
#include "view/layers/button.h"
#include "data/collection.h"
#include "view/theme.h"


namespace Layers {


List::~List()
{
    for (Layer* sub : m_Sublayers)
        sub->SetParent(nullptr);
}


Layer* List::GetFirstChild()
{
    m_CurrentChild = m_Sublayers.cbegin();
    if (m_CurrentChild != m_Sublayers.cend())
        return *m_CurrentChild;
    return nullptr;
}

Layer* List::GetNextChild()
{
    ++m_CurrentChild;
    if (m_CurrentChild != m_Sublayers.cend())
        return *m_CurrentChild;
    return nullptr;
}


Layer* List::GetChild(const std::string& name) const
{
    Collection* listData = dynamic_cast<Collection*>(GetContent());
    if (listData)
        for (unsigned i = 0; i < m_Sublayers.size(); ++i)
            if (listData->GetChild(i + m_StartPosition)->GetName() == name)
                return m_Sublayers[i];
    return nullptr;
}


void List::Rebuild()
{
    m_Sublayers.clear();

    Collection* listData = dynamic_cast<Collection*>(GetContent());
    if (listData)
    {
        unsigned count = listData->Size() - m_StartPosition;
        if (GetListLayout()->GetMaxItemsToShow() < count)
            count = GetListLayout()->GetMaxItemsToShow();

        for (unsigned i = 0; i < count; ++i)

            // Create the sub-objects
            AddLayer(listData->GetChild(i + m_StartPosition));
    }
    else
        AddLayer(GetContent());
}


void List::AddLayer(HmiItem* data)
{
    if (m_ExpandChildren)
        Insert(data->GetLayer());
    else
    {
        Layer* button = new Button();
        button->SetContent(data);
        Insert(button);
    }
}


Layer* List::Insert(Layer* a_child)
{
    m_Sublayers.push_back(a_child);
    a_child->SetParent(this);
    m_IsModified = true;
    return a_child;
}


void List::Remove(Layer* a_child)
{
    for (unsigned i = 0; i < m_Sublayers.size(); ++i)
    {
        if (m_Sublayers[i] == a_child)
        {
            m_Sublayers.erase(m_Sublayers.begin() + i);
            m_IsModified = true;
            return;
        }
    }
}


Drawer* List::CreatetDrawer()
{
    return GetTheme()->GetListDrawer();
}


bool List::SetFocus(HmiItem*)
{
    HmiItem* tgt= GetControlsList();
    if (!tgt)
        return false;

    // TODO
    return true;
}


HmiItem* List::GetControlsList()
{
    if (!m_Data)
        return nullptr;

    // TODO
    return nullptr;
}


void List::SetExpandChildren(bool expand)
{
    if (m_ExpandChildren != expand)
    {
        m_ExpandChildren = expand;
        m_ModifiedContent = true;
    }
}

}
