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
    delete m_Shortcut;
}


Interface* Item::GetInterface()
{
    if (m_Parent)
        return m_Parent->GetInterface();
    return nullptr;
}


void Item::SetSelected(bool isSelected)
{
    m_IsSelected = isSelected;
    SelectionListeners.Execute(this);
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
    if (m_ExpandedView)
        m_ExpandedView->SetData(this);
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
    m_ButtonView->SetData(this);
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


void Item::GetShortcuts(Hmi::Interface* caller, Hmi::List* out)
{
    if (m_Shortcut && !m_Shortcut->keys.empty())
        out->Attach(this);
}

}
