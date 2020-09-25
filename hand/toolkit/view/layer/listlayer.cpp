#include "view/layer/listlayer.h"
#include "graph/collection.h"
#include "view/theme.h"


ListLayer::~ListLayer()
{
    for (Layer* sub : m_Sublayers)
        sub->SetParent(nullptr);
}


void ListLayer::DrawChildren(bool forced)
{
    for (Layer* sub : GetSubLayers())
        GetDrawer()->DrawChild(sub, forced);
}


void ListLayer::Rebuild()
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
            Insert(listData->GetChild(i + m_StartPosition)->GetLayer());
    }
    else
        Insert(GetContent()->GetLayer());
}


Layer* ListLayer::Insert(Layer* a_child)
{
    m_Sublayers.push_back(a_child);
    a_child->SetParent(this);
    m_IsChanged = true;
    return a_child;
}


void ListLayer::Remove(Layer* a_child)
{
    for (unsigned i = 0; i < m_Sublayers.size(); ++i)
    {
        if (m_Sublayers[i] == a_child)
        {
            m_Sublayers.erase(m_Sublayers.begin() + i);
            m_IsChanged = true;
            return;
        }
    }
}


void ListLayer::UpdateSubContent()
{
    for (Layer* sub : m_Sublayers)
        m_IsChanged |= sub->Update();
}


Drawer* ListLayer::CreatetDrawer()
{
    return GetTheme()->GetListDrawer();
}


bool ListLayer::SetFocus(HmiItem*)
{
    HmiItem* tgt= GetControlsList();
    if (!tgt)
        return false;

    // TODO
    return true;
}


HmiItem* ListLayer::GetControlsList()
{
    if (!m_Data)
        return nullptr;

    // TODO
    return nullptr;
}
