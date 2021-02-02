#include "data/item.h"
#include "data/list.h"
#include "view/layer.h"
#include "view/layers/button.h"


namespace Hmi {


Item::~Item()
{
    if (m_Parent)
    {
        List* tmp = m_Parent;
        m_Parent = nullptr;
        tmp->Remove(this);
    }

    delete m_ExpandedView;
    delete m_ButtonView;

    for (ICallback* ic : m_SelectionChange)
        delete ic;
    for (ICallback* ic : m_Activation)
        delete ic;
    for (ICallback* ic : m_DataChanged)
        delete ic;

    delete m_Shortcut;
}


Interface* Item::GetParentInterface()
{
    if (m_Parent)
        return m_Parent->GetInterface();
    return nullptr;
}


void Item::SetSelected(bool isSelected)
{
    m_IsSelected = isSelected;
    Execute(m_SelectionChange);
}


void Item::RemoveCallback(ICallback* callback, Listeners& list)
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


void Item::Execute(const Listeners& list)
{
    for (ICallback* listener : list)
        listener->Execute(this);
}


Layer* Item::GetExpandedView()
{
    if (!m_ExpandedView)
        SetExpandedView(CreateExpandedView());
    return m_ExpandedView;
}


void Item::SetExpandedView(Layer* layer)
{
    delete m_ExpandedView;
    m_ExpandedView = layer;
    m_ExpandedView->SetContent(this);
}


Layer* Item::GetButtonView()
{
    if (!m_ButtonView)
        SetButtonView(CreateButtonView());
    return m_ButtonView;
}


void Item::SetButtonView(Layer* layer)
{
    delete m_ButtonView;
    m_ButtonView = layer;
    m_ButtonView->SetContent(this);
}


Layer* Item::CreateButtonView()
{
    return new Layers::Button();
}


void Item::SetShortcut(Chord* chord)
{
    delete m_Shortcut;
    m_Shortcut = chord;
}

}
