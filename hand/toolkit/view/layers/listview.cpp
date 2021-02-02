#include "view/layers/listview.h"
#include "view/layouts/builtin.h"
#include "data/method.h"


namespace Layers {


void ListView::SetContent(Hmi::Item* data)
{
    if (m_Data && !m_Back)
    {
        m_Back = new Hmi::Action<ListView>("Back", "Previous list", this, &ListView::Back);
        GetLayerControls()->Add(m_Back);
    }
    m_ViewStack.push_back(data);
    m_DataControls.SetContent(data);
    Map::SetContent(data);
}


void ListView::Rebuild()
{
    Map::Rebuild();

    Insert(VIEW, &m_DataControls);
    Insert(LAYER_CONTROLS, GetLayerControls()->GetExpandedView());
}


Drawer* ListView::CreatetDrawer() { return GetTheme()->GetViewDrawer(); }
Layout* ListView::CreateLayout() { return Layouts::CreateListView(); }


void ListView::Back(Hmi::Item*)
{
    m_ViewStack.pop_back();
    Map::SetContent(m_ViewStack.back());
    m_DataControls.SetContent(m_Data);
    if (m_ViewStack.size() == 1)
    {
        delete m_Back;
        m_Back = nullptr;
    }
}

}
