#ifndef HAND_VIEW_LAYOUT_H
#define HAND_VIEW_LAYOUT_H

#include "view/datatypes/rect.h"


#define CONTROL "Control"
#define DESCRIPTION "Description"
#define ICON "Icon"
#define TITLE "Title"
#define VIEW "View"


class Field;

class Layout
{
public:
    virtual ~Layout() = default;

    virtual Field* GetField(const std::string& name) const { return nullptr; }
    virtual SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) {
        return { offset.x, offset.y, 0, 0 };
    }
};


class Field : public Layout
{
public:
    Field(const std::string& name) : m_Name(name) {}

    /// Returns the size from the matching sub-layer.
    SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) override;
    // Returns this if 'name' matches this, returns NULL otherwise.
    Field* GetField(const std::string& name) const override;

    void SetVisible(bool visible) { m_IsVisble = visible; }
    bool IsVisible() { return m_IsVisble; }

private:
    std::string m_Name;
    bool m_IsVisble;
};


namespace Layouts {

enum Orientation
{
    Auto,
    Horizontal,
    Vertical
};


class List : public Layout
{
public:
    virtual SDL_Rect GetSize(Layer* tgt, SDL_Rect offset) = 0;

    unsigned GetMaxItemsToShow() { return m_MaxItemsToShow; }
    void SetMaxItemsToShow(unsigned count) { m_MaxItemsToShow = count; }

    Orientation GetOrientation() const { return m_Orientation; }
    void SetOrientation(Orientation value) { m_Orientation = value; }

private:
    Orientation m_Orientation = Horizontal;
    unsigned m_MaxItemsToShow = 5;
};

}
#endif //HAND_VIEW_LAYOUT_H
