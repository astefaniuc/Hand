#include "view/layer/listlayer.h"
#include "graph/collection.h"
#include "view/theme.h"


void ListLayer::Update()
{
    m_Sublayers.clear();
    Collection* listData = dynamic_cast<Collection*>(GetContent());
    if (listData)
    {
        unsigned count = listData->Size() - m_StartPosition;
        if (GetMaxItemsToShow() < count)
            count = GetMaxItemsToShow();

        for (unsigned i = 0; i < count; ++i)
            // Create the sub-objects
            Insert(listData->GetChild(i + m_StartPosition)->GetLayer());
    }
    else
        Insert(GetContent()->GetLayer());
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
