#include "view/layers/button.h"
#include "view/layers/text.h"
#include "view/theme.h"


namespace Layers {


void Button::Rebuild()
{
    Clear();

    m_Name = Insert(TITLE, new Text(m_Data->GetName()));
    if (!m_Data->GetDescription().empty())
        m_Description = Insert(DESCRIPTION, new Text(m_Data->GetDescription()));
    // TODO: m_Control
}


void Button::Clear()
{
    delete m_Name;
    delete m_Description;

    m_Description = nullptr;
    m_Name = nullptr;

    m_Sublayers.clear();
}


Drawer* Button::CreatetDrawer()
{
    return GetTheme()->GetButtonDrawer();
}

}
