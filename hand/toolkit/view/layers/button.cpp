#include "view/layers/button.h"
#include "view/layers/text.h"
#include "view/theme.h"


namespace Layers {


Drawer* Button::CreatetDrawer()
{
    return GetTheme()->GetButtonDrawer();
}

}
