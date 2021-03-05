#include "view/layers/hmi/item.h"
#include "view/layers/text.h"


namespace Layers {


void Item::Rebuild()
{
    Insert(TITLE, new Text(m_Data->GetName()));
    if (!m_Data->GetInfo().empty())
        Insert(DESCRIPTION, new Text(m_Data->GetInfo()));
}


void Item::SetData(Hmi::Item* data)
{
    List::SetData(data);
    ActivationListeners.Add(data, &Hmi::Item::Execute);
}


void Item::RemoveData()
{
    ActivationListeners.Remove(m_Data);
    List::RemoveData();
}

}
