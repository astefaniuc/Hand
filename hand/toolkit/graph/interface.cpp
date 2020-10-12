#include "graph/interface.h"
#include "view/layers/view.h"


Layer* Interface::CreateLayer()
{
    return new Layers::View();
}
