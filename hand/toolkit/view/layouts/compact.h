#ifndef HAND_VIEW_LAYOUTS_COMPACT_H
#define HAND_VIEW_LAYOUTS_COMPACT_H

#include "view/layout.h"


namespace Layouts { namespace Compact {

// "Link" for the Layer
class Map : public Layout
{
public:
    Map(Layout* field1, Layout* field2, Orientation orientation)
        : m_Field1(field1), m_Field2(field2), m_Orientation(orientation) {}

    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) override;
    Field* GetField(const std::string& name) const override;

protected:
    Layout* m_Field1;
    Layout* m_Field2;
    Orientation m_Orientation;
};


class List : public Layouts::List
{
public:
    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) override;
};



Layout* CreateButton();
Layout* CreateData();
Layout* CreateView();

Layout* AssureNode(Layout* in);
Layout* AssureNode(const std::string& in);

template<class T1, class T2>
Map* SplitV(T1 field1, T2 field2) {
    return new Map(AssureNode(field1), AssureNode(field2), Horizontal);
}

template<class T1, class T2>
Map* SplitH(T1* field1, T2* field2) {
    return new Map(AssureNode(field1), AssureNode(field2), Vertical);
}

}}
#endif //HAND_VIEW_LAYOUTS_COMPACT_H
