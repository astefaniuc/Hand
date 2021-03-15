#include "view/layers/hmi/listview.h"
#include "view/layouts/builtin.h"
#include "view/theme.h"
#include "data/method.h"


namespace Layers {


void ListView::SetData(Hmi::Item* data)
{
    Item::SetData(data);

    m_ViewStack.push_back(data);
    if (!m_Back && (m_ViewStack.size() > 1))
    {
        m_Back = new Hmi::Action<ListView>("Back", "Previous list", this, &ListView::Back);
        GetLayerControls()->Add(m_Back);
    }
}


void ListView::Rebuild()
{
    m_DataControls = m_Data->GetExpandedData()->GetListLayer();
    Insert(VIEW, m_DataControls);
}


Drawer* ListView::GetDrawerFromTheme() { return GetTheme()->GetViewDrawer(this); }
Layout* ListView::CreateLayout() { return Layouts::CreateListView(); }


void ListView::Back(Layers::Item*)
{
    m_ViewStack.pop_back();
    Item::SetData(m_ViewStack.back());
    if (m_ViewStack.size() == 1)
    {
        delete m_Back;
        m_Back = nullptr;
    }
}

}
