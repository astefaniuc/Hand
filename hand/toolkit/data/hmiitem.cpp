#include "data/hmiitem.h"
#include "data/collection.h"
#include "view/layer.h"
#include "view/layers/button.h"


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

    delete m_ExpandedView;
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


Layer* HmiItem::GetExpandedView()
{
    if (!m_ExpandedView)
        SetExpandedView(CreateExpandedView());
    return m_ExpandedView;
}


void HmiItem::SetExpandedView(Layer* layer)
{
    delete m_ExpandedView;
    m_ExpandedView = layer;
    m_ExpandedView->SetContent(this);
}


Layer* HmiItem::GetButtonView()
{
    if (!m_ButtonView)
        SetButtonView(CreateButtonView());
    return m_ButtonView;
}


void HmiItem::SetButtonView(Layer* layer)
{
    delete m_ButtonView;
    m_ButtonView = layer;
    m_ButtonView->SetContent(this);
}


Layer* HmiItem::CreateButtonView()
{
    return new Layers::Button();
}
