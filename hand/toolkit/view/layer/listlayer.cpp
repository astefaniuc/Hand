#include "view/layer/listlayer.h"
#include "graph/collection.h"


void ListLayer::SetContent(HmiItem* a_data)
{
    // Connect list and layer
    Layer::SetContent(a_data);
    m_StartPosition = 0;
}


void ListLayer::Show(HmiItem* a_hmi)
{
    Collection* listData = dynamic_cast<Collection*>(a_hmi);
    if (listData)
    {
        unsigned count = listData->Size() - m_StartPosition;
        if(GetMaxItemsToShow() < count)
            count = GetMaxItemsToShow();

        for (unsigned i = 0; i < count; ++i)
            // Create the sub-objects
            Insert(listData->GetChild(i + m_StartPosition)->GetLayer());
    }
    else
        Insert(a_hmi->GetLayer());
}


bool ListLayer::SetFocus(HmiItem*)
{
    HmiItem* tgt= GetControlsList();
    if(!tgt)
        return false;

    // TODO
    return true;
}


HmiItem* ListLayer::GetControlsList()
{
    if(!m_Data)
        return nullptr;

    // TODO
    return nullptr;
}
