#ifndef HAND_VIEW_LAYOUT_H
#define HAND_VIEW_LAYOUT_H

#include "view/datatypes/rect.h"
#include "include/stdfields.h"


namespace Layers { class List; }
namespace Layouts
{

enum EAlignment
{
    Top,
    Left,
    Center,
    Bottom,
    Right
};

struct SAlignment
{
    EAlignment Parent = Center;
    EAlignment Field = Center;
};

class Field;

}


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


    Layouts::Field* GetField(const std::string& name, bool create = true);
    void SetField(const std::string& name, Layouts::SAlignment alignment);

    virtual SDL_Rect GetSize(const SDL_Rect& outer) {
        return { outer.x, outer.y, 0, 0 };
    }

    bool IsValid();
    // Returns the vector size.
    unsigned GetValidFields(std::vector<Layouts::Field*>& out);

protected:
    std::vector<Layouts::Field*> m_Fields;
};


namespace Layouts {


/// Places the 'source' rect into the 'target' rect as specified with 'alignment'.
/// For EAlignment::Center it aligns in vertical and horizontal direction.
void Align(EAlignment alignment, const SDL_Rect& target, SDL_Rect& source);


class Field
{
public:
    Field(const std::string& name) : m_Name(name) { delete m_Layout; }

    /// Returns the size from the matching sub-layer.
    SDL_Rect GetSize(const SDL_Rect& outer);
    SDL_Rect GetAlignedSize(const SDL_Rect& outer);
    SDL_Rect GetPlacedSize(const SDL_Rect& outer);

    Field* GetField(const std::string& name) const;

    void SetLayout(Layout* layout);
    void SetLayer(Layer* layer) { m_Layer = layer; }

    void SetVisible(bool visible) { m_IsVisible = visible; }
    bool IsVisible() { return m_IsVisible; }

    SAlignment GetAlignment() { return m_Alignment; }
    void SetAlignment(SAlignment alignment) { m_Alignment = alignment;}

    void SetPosition(const RelRect& pos) { m_Position = pos;}

    bool IsValid() { return (m_Layer || (m_Layout && m_Layout->IsValid())); }

protected:
    Layout* m_Layout = nullptr;
    Layer* m_Layer = nullptr;

    bool m_IsVisible = true;
    SAlignment m_Alignment;
    RelRect m_Position;

private:
    std::string m_Name;
};


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
