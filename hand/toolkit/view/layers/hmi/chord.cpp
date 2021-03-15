#include "view/layers/hmi/chord.h"
#include "view/theme.h"
#include "view/layouts/builtin.h"


namespace Layers {


Drawer* Chord::GetDrawerFromTheme()
{
    return GetTheme()->GetChordDrawer(this);
}


Layout* Chord::CreateLayout()
{
    return new Layouts::List();
}

}
