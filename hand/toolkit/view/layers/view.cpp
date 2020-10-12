#include "view/layers/view.h"
#include "view/layers/text.h"
#include "graph/method.h"
#include "view/theme.h"


namespace Layers {


View::View()
{
    m_Exit = new Action<View>("Exit", "Close interface", this, &View::Exit);
}


void View::Rebuild()
{
    Clear();

    if (GetLayout()->GetField(TITLE)->IsVisible())
        m_Title = Insert(TITLE, new Text(m_Data->GetName()));

    if (GetLayout()->GetField(DESCRIPTION)->IsVisible())
        m_Description = Insert(DESCRIPTION, new Text(m_Data->GetDescription()));

    Interface* in = static_cast<Interface*>(m_Data);

    m_View = in->GetView();
    if (m_View)
        Insert(VIEW, m_View);

    HmiItem* controls = in->GetControls();
    if (controls)
        m_Control = Insert(CONTROL, controls->GetLayer());
}


Drawer* View::CreatetDrawer()
{
    return GetTheme()->GetViewDrawer();
}


void View::Clear()
{
    delete m_Title;
    delete m_Description;

    m_Control = nullptr;
    m_Description = nullptr;
    m_Title = nullptr;
    m_View = nullptr;

    m_Sublayers.clear();
}

}
