#include "data/interface.h"
#include "view/layers/view.h"


namespace Hmi {


Layer* Interface::CreateExpandedView()
{
    return new Layers::View();
}

}
