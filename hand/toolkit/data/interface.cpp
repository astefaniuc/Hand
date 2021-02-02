#include "data/interface.h"
#include "view/layers/interface.h"


namespace Hmi {


Layer* Interface::CreateExpandedView()
{
    return new Layers::Interface();
}


void Interface::ConnectButton(Layer* view)
{
//    AddActivationClient(CCallback<Interface>(this));
}

}
