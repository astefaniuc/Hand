#include "view/layers/hmi/data.h"
#include "view/layers/text.h"
#include "view/theme.h"


namespace Layers {


Drawer* Data::GetDrawerFromTheme()
{
    return GetTheme()->GetDataDrawer(this);
}

}
