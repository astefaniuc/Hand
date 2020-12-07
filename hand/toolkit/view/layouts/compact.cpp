#include "view/layouts/compact.h"
#include "view/layers/list.h"


namespace Layouts { namespace Compact {


Field* MakeField(Layout* in)
{
    Field* ret = new Field("");
    ret->SetLayout(in);
    return ret;
}
Field* MakeField(const std::string& in) { return new Field(in); }

Layout* Split(Field* field1, Field* field2, Layout::Orientation orientation)
{
    List* ret = new List();
    ret->SetField(field1);
    ret->SetField(field2);
    ret->SetOrientation(orientation);
    return ret;
}



Layout* CreateButton()
{
    return
        SplitV(
            SplitH(VIEW, TITLE),
            CONTROL);
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
    return
        SplitV(
            TITLE,
            SplitV(
                SplitH(VIEW, DESCRIPTION),
                CONTROL));
}

}}
