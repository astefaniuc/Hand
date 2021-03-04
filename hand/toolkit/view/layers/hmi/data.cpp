#include "view/layers/hmi/data.h"
#include "view/layers/text.h"
#include "view/theme.h"


namespace Layers {


void Data::Rebuild()
{
    Item::Rebuild();

    Insert(VIEW, new Text(static_cast<Hmi::Data*>(m_Data)->GetValueString()));
}


Drawer* Data::GetDrawerFromTheme()
{
    return GetTheme()->GetDataDrawer(this);
}

}
