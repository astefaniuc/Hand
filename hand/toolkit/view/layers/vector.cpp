#include "view/layers/vector.h"
#include "data/list.h"
#include "view/theme.h"


namespace Layers {


void Vector::Rebuild()
{
    if (!GetData())
        return;

    Data::List* listData = dynamic_cast<Data::List*>(GetData());
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


void Vector::AddLayer(Data::Item* data)
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

}
