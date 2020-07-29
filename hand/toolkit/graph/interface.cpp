#include "graph/interface.h"
#include "view/layer/masterlayer.h"


Layer* Interface::CreateLayer()
{
    return new MasterLayer();
}
