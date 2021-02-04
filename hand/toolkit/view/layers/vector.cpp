#include "view/layers/vector.h"
#include "view/layers/button.h"
#include "data/list.h"
#include "view/theme.h"


namespace Layers {


void Vector::Rebuild()
{
    m_Layout->ClearFields();

    std::vector<Hmi::Item*> items;
    GetActiveItems(items);

    for (Hmi::Item* item : items)
        // Create the sub-objects
        AddLayer(item);
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
    GetLayout()->GetField(a_child->GetData()->GetName())->SetItem(a_child);
    SetModified();
    return a_child;
}


void Vector::GetActiveItems(std::vector<Hmi::Item*>& out)
{
    if (!GetData())
        return;

    Hmi::List* listData = dynamic_cast<Hmi::List*>(GetData());
    if (listData)
    {
        unsigned count = listData->Size() - m_StartPosition;
        if (GetMaxItemsToShow() < count)
            count = GetMaxItemsToShow();

        for (unsigned i = 0; i < count; ++i)

            // Create the sub-objects
            out.push_back(listData->GetChild(i + m_StartPosition));
    }
    else
        out.push_back(GetData());
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
