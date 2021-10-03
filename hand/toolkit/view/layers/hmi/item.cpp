#include "view/layers/hmi/item.h"
#include "view/layers/hmi/data.h"
#include "view/layers/text.h"


void Layers::Item::Rebuild()
{
    Insert(TITLE, new RawText(m_Data->GetName()));
    if (!m_Data->GetInfo().empty())
        Insert(DESCRIPTION, new RawText(m_Data->GetInfo()));

    Layer* data = nullptr;
    if (m_ExpandChildren)
        data = m_Data->GetExpandedData();
    else
        data = m_Data->GetCompressedData();

    if (data)
    {
        Data* frame = new Data();
        frame->Insert(data);
        Insert(VIEW, frame);
    }
}


void Layers::Item::SetData(::Data::Item* data)
{
    List::SetData(data);
    ActivationListeners.Add(data, &::Data::Item::Execute);
}


void Layers::Item::RemoveData()
{
    ActivationListeners.Remove(m_Data);
    List::RemoveData();
}
