#include "view/layers/hmi/data.h"
#include "view/layers/text.h"
#include "view/theme.h"


namespace Layers {


void Data::Rebuild()
{
    Item::Rebuild();

    m_DataText.SetData(static_cast<Hmi::Data*>(m_Data)->GetValueString());
    Insert(VIEW, &m_DataText);
}


Drawer* Data::GetDrawerFromTheme()
{
    return GetTheme()->GetDataDrawer(this);
}

}
