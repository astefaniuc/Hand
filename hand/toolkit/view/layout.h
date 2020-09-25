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

//    virtual SDL_Rect GetSize(Layer* tgt) = 0;
    virtual Field* GetField(const std::string& name) const { return nullptr; }
};


class MapNode : public Node
{
public:
    virtual SDL_Rect GetFieldSize(LayerMap* tgt, SDL_Rect offset) = 0;
};

// "Link" for the Layer
class Separator : public MapNode
{
public:
    Separator(MapNode* field1, MapNode* field2, Orientation orientation)
        : m_Field1(field1), m_Field2(field2), m_Orientation(orientation) {}

    SDL_Rect GetFieldSize(LayerMap* tgt, SDL_Rect offset) override;
    Field* GetField(const std::string& name) const override;

protected:
    MapNode* m_Field1;
    MapNode* m_Field2;
    Orientation m_Orientation;
};


class Field : public MapNode
{
public:
    Field(const std::string& name) : m_Name(name) {}

    SDL_Rect GetFieldSize(LayerMap* tgt, SDL_Rect offset) override;
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
    SDL_Rect GetFieldSize(ListLayer* tgt, SDL_Rect offset);

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

MapNode* AssureNode(MapNode* in);
MapNode* AssureNode(const std::string& in);

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
