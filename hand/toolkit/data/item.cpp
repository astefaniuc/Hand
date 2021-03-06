#include "data/item.h"
#include "data/list.h"
#include "view/layers/hmi/button.h"


namespace Hmi {


Item::~Item()
{
    if (m_Parent)
    {
        List* tmp = m_Parent;
        m_Parent = nullptr;
        tmp->Remove(this);
    }

    ExitListeners.Notify(this);

    delete m_LayerInitializer;
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
    SelectionListeners.Notify(this);
}


Layer* Item::GetExpandedView()
{
    Layer* ret = CreateExpandedView();
    if (m_LayerInitializer)
        m_LayerInitializer->Execute(ret);
    ret->SetData(this);
    return ret;
}


Layer* Item::GetButtonView()
{
    Layer* ret = CreateButtonView();
    ret->SetData(this);
    return ret;
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
    if (m_Shortcut && !m_Shortcut->Keys.empty())
        out->Attach(this);
}

}
