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


class Field;

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


    Field* GetField(const std::string& name, bool create = true);
    void SetField(Field* field) { m_Fields.push_back(field); }
    void SetField(
        const std::string& name,
        VAlignment::Position vertical,
        HAlignment::Position horizontal);

    virtual SDL_Rect GetSize(const SDL_Rect& outer) {
        return { outer.x, outer.y, 0, 0 };
    }
    virtual void UpdatePositions(const SDL_Rect& outer) = 0;

    bool IsValid();
    virtual bool IsExpanding(Orientation direction);

    /// Returns the vector size.
    unsigned GetValidFields(std::vector<Field*>& out);

protected:
    std::vector<Field*> m_Fields;
};


namespace Layouts {


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
    void UpdatePositions(const SDL_Rect& outer) override;

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

}
#endif //HAND_VIEW_LAYOUT_H
