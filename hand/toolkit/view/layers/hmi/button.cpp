#include "view/layers/hmi/button.h"
#include "view/theme.h"


namespace Layers {


Drawer* Button::GetDrawerFromTheme()
{
    return GetTheme()->GetButtonDrawer(this);
}

}
