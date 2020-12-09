#include "view/layouts/builtin.h"
#include "view/field.h"


namespace Layouts {


Layout* CreateButton()
{
    return
        SplitV(
            SplitH(VIEW, TITLE),
            SplitH(DESCRIPTION, CONTROL));
}


Layout* CreateData()
{
    return
        SplitV(
            SplitH(TITLE, VIEW),
            SplitH(DESCRIPTION, CONTROL));
}


Layout* CreateView()
{
    List* ret = new List();

    ret->SetOrientation(Layout::Vertical);
    ret->GetField(TITLE);
    ret->GetField(VIEW);
    ret->GetField(DESCRIPTION);
    ret->SetField(MakeField(SplitH(CONTROL, LAYER_CONTROLS)));

    return ret;
}

}