#include "data/manipulator/manipulator.h"
#include "view/layers/hmi/interface.h"


namespace Hmi {


void Show(Layer* target, Interface* view)
{
    target->GetInterface()->Show(view->GetExpandedView());
}

}
