#include "view/layouts/builtin.h"
#include "view/field.h"


namespace Layouts {


Layout* CreateButton()
{
    return
        SplitV(
            SplitH(TITLE, VIEW),
            SplitH(DESCRIPTION, CHORD));
}


Layout* CreateListView()
{
    return SplitH(VIEW, LAYER_CONTROLS);
}


Layout* CreateView()
{
    List* ret = new List();

    ret->SetOrientation(Vertical);
    ret->GetField(TITLE);
    ret->GetField(VIEW);
    ret->GetField(DESCRIPTION);
    ret->SetField(MakeField(SplitH(CONTROLS, LAYER_CONTROLS)));

    return ret;
}

}
