#include "view/layers/data.h"
#include "view/layers/text.h"
#include "view/theme.h"


namespace Layers {


void Data::Rebuild()
{
    Map::Rebuild();

    m_DataText.SetData(static_cast<Hmi::Data*>(m_Data)->GetValueString());
    Insert(VIEW, &m_DataText);
}


Drawer* Data::CreatetDrawer()
{
    return GetTheme()->GetDataDrawer(this);
}

}
