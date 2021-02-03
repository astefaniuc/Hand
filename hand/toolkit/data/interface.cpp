#include "data/interface.h"
#include "view/layers/interface.h"


namespace Hmi {


void Interface::GetShortcuts(Hmi::Interface* caller, Hmi::List* out)
{
    if (caller != this)
        // Don't descend into sub-interfaces. TODO: exported shortcuts.
        return Item::GetShortcuts(caller, out);

    Map::GetShortcuts(caller, out);
}


Layer* Interface::CreateExpandedView()
{
    return new Layers::Interface();
}


void Interface::ConnectButton(Layer* view)
{
//    AddActivationClient(CCallback<Interface>(this));
}

}
