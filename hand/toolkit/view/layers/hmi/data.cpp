#include "view/layers/hmi/data.h"
#include "view/theme.h"
#include "view/layouts/builtin.h"


namespace Layers {


Drawer* Data::GetDrawerFromTheme()
{
    return GetTheme()->GetDataDrawer(this);
}


Layout* Data::CreateLayout()
{
    return new Layouts::List();
}

}
