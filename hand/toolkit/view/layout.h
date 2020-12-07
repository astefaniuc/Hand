#ifndef HAND_VIEW_LAYOUT_H
#define HAND_VIEW_LAYOUT_H

#include "view/datatypes/rect.h"
#include "include/stdfields.h"


struct VAlignment
{
    enum Position
    {
        Top,
        VCenter,
        Bottom,
    };
    /// Places the 'source' rect into the 'target' rect as specified with 'alignment'.
    void Align(const SDL_Rect& target, SDL_Rect& source);

    Position Pos = VCenter;
};


struct HAlignment
{
    enum Position
    {
        Left,
        HCenter,
        Right
    };
    /// Places the 'source' rect into the 'target' rect as specified with 'alignment'.
    void Align(const SDL_Rect& target, SDL_Rect& source);

    Position Pos = HCenter;
};


namespace Layers { class List; }
namespace Layouts{ class Field; }

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
    void SetField(Layouts::Field* field) { m_Fields.push_back(field); }
    void SetField(
        const std::string& name,
        VAlignment::Position vertical,
        HAlignment::Position horizontal);

    virtual SDL_Rect GetSize(const SDL_Rect& outer) {
        return { outer.x, outer.y, 0, 0 };
    }

    bool IsValid();
    virtual bool IsExpanding(Orientation direction);

    /// Returns the vector size.
    unsigned GetValidFields(std::vector<Layouts::Field*>& out);

protected:
    std::vector<Layouts::Field*> m_Fields;
};


namespace Layouts {


class Field
{
public:
    Field(const std::string& name) : m_Name(name) { delete m_Layout; }

    /// Returns the size from the matching sub-layer.
    SDL_Rect GetSize(const SDL_Rect& outer);
    SDL_Rect GetPlacedSize(const SDL_Rect& outer);

    Field* GetField(const std::string& name) const;

    void SetLayout(Layout* layout);
    void SetLayer(Layer* layer) { m_Layer = layer; }

    void SetVisible(bool visible) { m_IsVisible = visible; }
    bool IsVisible() { return m_IsVisible; }

    void SetAlignment(VAlignment::Position position) { m_AlignmentV.Pos = position;}
    void SetAlignment(HAlignment::Position position) { m_AlignmentH.Pos = position;}
    void SetAlignment(VAlignment::Position vertical, HAlignment::Position horizontal);
    void Align();

    void SetPosition(const RelRect& pos) { m_Position = pos;}

    bool IsValid() { return (m_Layer || (m_Layout && m_Layout->IsValid())); }

    void SetExpanding(bool vertical, bool horizontal);
    bool IsExpanding(Layout::Orientation direction);

    SDL_Rect Size;
    SDL_Rect Frame;

protected:
    Layout* m_Layout = nullptr;
    Layer* m_Layer = nullptr;

    bool m_IsVisible = true;
    bool m_ExpandV = false;
    bool m_ExpandH = false;
    VAlignment m_AlignmentV;
    HAlignment m_AlignmentH;
    RelRect m_Position;

private:
    std::string m_Name;
};


class List : public Layout
{
public:
    enum Expansion
    {
        Compact,
        EqualSize,
        EqualSpace
    };

    SDL_Rect GetSize(const SDL_Rect& outer) override;

    void SetExpansion(Expansion mode) { m_ExpansionMode = mode; }

    unsigned GetMaxItemsToShow() { return m_MaxItemsToShow; }
    void SetMaxItemsToShow(unsigned count) { m_MaxItemsToShow = count; }

    Orientation GetOrientation() const { return m_Orientation; }
    void SetOrientation(Orientation value) { m_Orientation = value; }

private:
    void SetExpandedSize(SDL_Rect outer, const std::vector<Field*>& fields);
    void SetEqualSize(const std::vector<Field*>& fields);
//    SDL_Rect SetEqualSpace(const std::vector<Field*>& fields);
    SDL_Rect GetCompactSize(const std::vector<Field*>& fields);
    uint16_t SetSameSize(const std::vector<Field*>& fields, Layout::Orientation orientation);

    Orientation m_Orientation = Horizontal;
    unsigned m_MaxItemsToShow = 5;

    Expansion m_ExpansionMode = Compact;
};

}
#endif //HAND_VIEW_LAYOUT_H
