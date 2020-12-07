#include "view/layouts/aligned.h"
#include "view/layouts/compact.h"
#include "view/layers/list.h"


namespace Layouts { namespace Aligned {


List* CreateView()
{
    List* ret = new List();
    ret->SetOrientation(Layout::Vertical);
    ret->SetField(TITLE, VAlignment::VCenter, HAlignment::HCenter);
    {
        List* middle = new List();
        middle->SetOrientation(Layout::Horizontal);
        {
            Field* fExtView = middle->GetField("extView", true);
            fExtView->SetAlignment(VAlignment::VCenter, HAlignment::HCenter);
            fExtView->SetExpanding(true, true);
            {
                List* extView = new List();
                extView->SetOrientation(Layout::Vertical);
                extView->SetField(VIEW, VAlignment::VCenter, HAlignment::HCenter);
                extView->SetField(DESCRIPTION, VAlignment::VCenter, HAlignment::HCenter);
                fExtView->SetLayout(extView);
            }
        }
        middle->GetField(LAYER_CONTROLS, true);
        ret->GetField("middle")->SetLayout(middle);
    }
    ret->GetField(CONTROL, true);

    return ret;
}

}}
