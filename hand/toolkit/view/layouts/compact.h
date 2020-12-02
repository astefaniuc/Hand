#ifndef HAND_VIEW_LAYOUTS_COMPACT_H
#define HAND_VIEW_LAYOUTS_COMPACT_H

#include "view/layout.h"


namespace Layouts { namespace Compact {


class Map : public Layout
{
public:
    Map(Field* field1, Field* field2, Orientation orientation)
        : m_Orientation(orientation)
    {
        m_Fields.push_back(field1);
        m_Fields.push_back(field2);
    }

    SDL_Rect GetSize(SDL_Rect& outer) override;

protected:
    Orientation m_Orientation;
};


class List : public Layouts::List
{
public:
    SDL_Rect GetSize(SDL_Rect& outer) override;
};



Layout* CreateButton();
Layout* CreateData();
Layout* CreateView();

Field* AssureNode(Layout* in);
Field* AssureNode(const std::string& in);

template<class T1, class T2>
Map* SplitV(T1 field1, T2 field2) {
    return new Map(AssureNode(field1), AssureNode(field2), Layout::Horizontal);
}

template<class T1, class T2>
Map* SplitH(T1* field1, T2* field2) {
    return new Map(AssureNode(field1), AssureNode(field2), Layout::Vertical);
}

}}
#endif //HAND_VIEW_LAYOUTS_COMPACT_H
