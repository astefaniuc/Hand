#include "view/layer/listlayer.h"
#include "graph/collection.h"
#include "view/theme.h"


void ListLayer::Rebuild()
{
    m_Sublayers.clear();

    ListLayout* layout = dynamic_cast<ListLayout*>(GetLayout());
    Collection* listData = dynamic_cast<Collection*>(GetContent());
    if (listData)
    {
        unsigned count = listData->Size() - m_StartPosition;
        if (layout->GetMaxItemsToShow() < count)
            count = layout->GetMaxItemsToShow();

        for (unsigned i = 0; i < count; ++i)
            // Create the sub-objects
            Insert(listData->GetChild(i + m_StartPosition)->GetLayer());
    }
    else
        Insert(GetContent()->GetLayer());

    for (unsigned i = 0; i < m_Sublayers.size(); ++i)
        SetSubSize(m_Sublayers[i], layout->GetField(i, m_Sublayers.size()));
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
