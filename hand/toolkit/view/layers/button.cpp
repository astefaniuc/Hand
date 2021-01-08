#include "view/layers/button.h"
#include "view/layers/text.h"
#include "view/theme.h"


namespace Layers {


void Button::Rebuild()
{
    Map::Rebuild();
}


Drawer* Button::CreatetDrawer()
{
    return GetTheme()->GetButtonDrawer();
}

}
