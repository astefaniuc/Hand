#include "view/layer/textlayer.h"
#include "view/theme.h"


Drawer* TextLayer::CreatetDrawer()
{
    return GetTheme()->GetTextDrawer();
}
