#include "data/manipulator/manipulator.h"
#include "view/layers/hmi/interface.h"


namespace Manipulator {


void Base::GetHmi(Layer* target)
{
    if (m_Interface)
        target->GetInterface()->Show(m_Interface->GetExpandedView());
}

}
