#include "data/interface.h"
#include "view/layers/view.h"


Layer* Interface::CreateExpandedView()
{
    return new Layers::View();
}
