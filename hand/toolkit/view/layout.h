#ifndef HAND_VIEW_LAYOUT_H
#define HAND_VIEW_LAYOUT_H

#include "view/datatypes/rect.h"
#include "base/module.h"


#define CONTROL "Control"
#define DESCRIPTION "Description"
#define ICON "Icon"
#define TITLE "Title"
#define VIEW "View"


class LayerMap;
class ListLayer;

namespace Layout {


enum Orientation
{
    Auto,
    Horizontal,
    Vertical
};


class Field;

class Node
{
public:
    virtual ~Node() = default;

    virtual Field* GetField(const std::string& name) const { return nullptr; }
    virtual SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) {
        return { offset.x, offset.y, 0, 0 };
    }
};

// "Link" for the Layer
class Separator : public Node
{
public:
    Separator(Node* field1, Node* field2, Orientation orientation)
        : m_Field1(field1), m_Field2(field2), m_Orientation(orientation) {}

    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) override;
    Field* GetField(const std::string& name) const override;

protected:
    Node* m_Field1;
    Node* m_Field2;
    Orientation m_Orientation;
};


class Field : public Node
{
public:
    Field(const std::string& name) : m_Name(name) {}

    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) override;
    Field* GetField(const std::string& name) const override;

    void SetVisible(bool visible) { m_IsVisble = visible; }
    bool IsVisible() { return m_IsVisble; }

private:
    std::string m_Name;
    bool m_IsVisble;
};


class List : public Node
{
public:
    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset);

    void SetMaxItemsToShow(unsigned count) { m_MaxItemsToShow->SetValue(count); }
    unsigned GetMaxItemsToShow() { return m_MaxItemsToShow->GetValue(); }

    Orientation GetOrientation() const { return m_Orientation->GetValue(); }
    void SetOrientation(Orientation value) { m_Orientation->SetValue(value); }

private:
    TData<Orientation>* m_Orientation = new TData<Orientation>("Orientation", "", Horizontal);
    TData<unsigned>* m_MaxItemsToShow = new TData<unsigned>("List max", "Items to show", 5);
};



Node* CreateButton();
Node* CreateData();
Node* CreateView();

Node* AssureNode(Node* in);
Node* AssureNode(const std::string& in);

template<class T1, class T2>
Separator* SplitV(T1 field1, T2 field2) {
    return new Separator(AssureNode(field1), AssureNode(field2), Horizontal);
}

template<class T1, class T2>
Separator* SplitH(T1* field1, T2* field2) {
    return new Separator(AssureNode(field1), AssureNode(field2), Vertical);
}

}

#endif //HAND_VIEW_LAYOUT_H
