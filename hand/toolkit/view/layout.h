#ifndef HAND_VIEW_LAYOUT_H
#define HAND_VIEW_LAYOUT_H

#include "view/datatypes/rect.h"
#include "include/stdfields.h"


class Field;
namespace Layers { class List; }


class Layout
{
public:
    enum Orientation
    {
        Auto,
        Horizontal,
        Vertical
    };

    virtual ~Layout() = default;

    virtual Field* GetField(const std::string& name) const { return nullptr; }
    virtual SDL_Rect GetSize(Layers::List* tgt, SDL_Rect& outer) {
        return { outer.x, outer.y, 0, 0 };
    }
};


class Field : public Layout
{
public:
    Field(const std::string& name) : m_Name(name) {}

    /// Returns the size from the matching sub-layer.
    SDL_Rect GetSize(Layers::List* parent, SDL_Rect& outer) override {
        return GetLayerSize(parent, outer);
    }
    // Returns this if 'name' matches this, returns NULL otherwise.
    Field* GetField(const std::string& name) const override;

    void SetVisible(bool visible) { m_IsVisible = visible; }
    bool IsVisible() { return m_IsVisible; }

protected:
    SDL_Rect GetLayerSize(Layers::List* parent, SDL_Rect& outer);

private:
    std::string m_Name;
    bool m_IsVisible = true;
};


namespace Layouts {

class List : public Layout
{
public:
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
