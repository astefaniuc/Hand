#include "data/hmiitem.h"
#include "data/collection.h"
#include "view/layer.h"


HmiItem::~HmiItem()
{
    for (ICallback* ic : m_SelectionChange)
        delete ic;
    for (ICallback* ic : m_Activation)
        delete ic;

    if (m_Parent)
    {
        Collection* tmp = m_Parent;
        m_Parent = nullptr;
        tmp->Remove(this);
    }

    delete m_Layer;
}


void HmiItem::SetSelected(bool isSelected)
{
    m_IsSelected = isSelected;
    Execute(m_SelectionChange);
}


void HmiItem::RemoveCallback(ICallback* callback, Listeners& list)
{
    // TODO: Do we need thread safety here?
    for (unsigned i = 0; i < list.size(); ++i)
    {
        // TODO: a memory compare may be a safer way to check for equality.
        if (list[i] == callback)
        {
            list.erase(list.begin() + i);
            return;
        }
    }
}


void HmiItem::Execute(const Listeners& list)
{
    for (ICallback* listener : list)
        listener->Execute(this);
}


Layer* HmiItem::GetLayer()
{
    if (!m_Layer)
        SetLayer(CreateLayer());
    return m_Layer;
}


void HmiItem::SetLayer(Layer* layer)
{
    delete m_Layer;
    m_Layer = layer;
    m_Layer->SetContent(this);
}
