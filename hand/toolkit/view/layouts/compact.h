#ifndef HAND_VIEW_LAYOUTS_COMPACT_H
#define HAND_VIEW_LAYOUTS_COMPACT_H

#include "view/layout.h"


namespace Layouts { namespace Compact {


Layout* CreateButton();
Layout* CreateData();
Layout* CreateView();

Field* MakeField(Layout* in);
Field* MakeField(const std::string& in);
Layout* Split(Field* field1, Field* field2, Layout::Orientation orientation);

template<class T1, class T2>
Layout* SplitV(T1 field1, T2 field2) {
    return Split(MakeField(field1), MakeField(field2), Layout::Vertical);
}

template<class T1, class T2>
Layout* SplitH(T1* field1, T2* field2) {
    return Split(MakeField(field1), MakeField(field2), Layout::Horizontal);
}

}}
#endif //HAND_VIEW_LAYOUTS_COMPACT_H
