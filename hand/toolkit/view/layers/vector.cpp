#include "view/layers/vector.h"
#include "view/layers/button.h"
#include "data/list.h"
#include "view/theme.h"


namespace Layers {


void Vector::Rebuild()
{
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
    SetModified();
    return a_child;
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
