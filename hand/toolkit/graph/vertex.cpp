#include "graph/vertex.h"
#include "view/layer/layer.h"


HmiItem::~HmiItem()
{
    delete m_Visualization;
}


void HmiItem::SetSelected(bool a_isSelected)
{
    m_IsSelected = a_isSelected;
    Execute(m_SelectionChange);
}


void HmiItem::RemoveCallback(ICallback* a_callback, Listeners& a_list)
{
    // TODO: Do we need thread safety here?
    for (unsigned i = 0; i < a_list.size(); ++i)
    {
        // TODO: a memory compare may be a safer way to check for equality.
        if (a_list[i] == a_callback)
        {
            a_list.erase(a_list.begin() + i);
            return;
        }
    }
}


void HmiItem::Execute(const Listeners& a_list)
{
    for (ICallback* listener : a_list)
        listener->Execute(this);
}


Layer* HmiItem::GetLayer()
{
    if (!m_Visualization)
        m_Visualization = CreateLayer();
    return m_Visualization;
}


void HmiItem::SetLayer(Layer* a_visualization)
{
    delete m_Visualization;
    m_Visualization = a_visualization;
}
