#include "view/layers/vector.h"
#include "view/layers/hmi/button.h"
#include "data/list.h"
#include "view/theme.h"


namespace Layers {


void Vector::Rebuild()
{
    m_Layout->ClearFields();

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
            AddLayer(listData->GetChild(i + m_StartPosition));
    }
    else
        AddLayer(GetData());
}


void Vector::AddLayer(Hmi::Item* data)
{
    if (m_ExpandChildren)
        Insert(data->GetExpandedView());
    else
        Insert(data->GetButtonView());
}


Drawer* Vector::GetDrawerFromTheme()
{
    return GetTheme()->GetListDrawer(this);
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
