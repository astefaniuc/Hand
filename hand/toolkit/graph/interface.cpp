#include "graph/interface.h"
#include "view/layer/layermanager.h"


Layer* Interface::CreateLayer()
{
    return new MasterLayer();
}
