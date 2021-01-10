#include "data/interface.h"
#include "view/layers/view.h"


namespace Hmi {


Layer* Interface::CreateExpandedView()
{
    return new Layers::View();
}


void Interface::ConnectButton(Layer* view)
{
//    AddActivationClient(CCallback<Interface>(this));
}

}
