#ifndef HAND_VIEW_LAYOUTS_COMPACT_H
#define HAND_VIEW_LAYOUTS_COMPACT_H

#include "view/layout.h"


#define CONTROL "Control"
#define DESCRIPTION "Description"
#define ICON "Icon"
#define TITLE "Title"
#define VIEW "View"


namespace Layouts {

// "Link" for the Layer
class Separator : public Layout
{
public:
    Separator(Layout* field1, Layout* field2, Orientation orientation)
        : m_Field1(field1), m_Field2(field2), m_Orientation(orientation) {}

    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) override;
    Field* GetField(const std::string& name) const override;

protected:
    Layout* m_Field1;
    Layout* m_Field2;
    Orientation m_Orientation;
};


class CompactList : public List
{
public:
    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset)override;

    void SetMaxItemsToShow(unsigned count) { m_MaxItemsToShow->SetValue(count); }
    unsigned GetMaxItemsToShow() { return m_MaxItemsToShow->GetValue(); }

    Orientation GetOrientation() const { return m_Orientation->GetValue(); }
    void SetOrientation(Orientation value) { m_Orientation->SetValue(value); }

private:
    TData<Orientation>* m_Orientation = new TData<Orientation>("Orientation", "", Horizontal);
    TData<unsigned>* m_MaxItemsToShow = new TData<unsigned>("List max", "Items to show", 5);
};



Layout* CreateButton();
Layout* CreateData();
Layout* CreateView();

Layout* AssureNode(Layout* in);
Layout* AssureNode(const std::string& in);

template<class T1, class T2>
Separator* SplitV(T1 field1, T2 field2) {
    return new Separator(AssureNode(field1), AssureNode(field2), Horizontal);
}

template<class T1, class T2>
Separator* SplitH(T1* field1, T2* field2) {
    return new Separator(AssureNode(field1), AssureNode(field2), Vertical);
}

}
#endif //HAND_VIEW_LAYOUTS_COMPACT_H
