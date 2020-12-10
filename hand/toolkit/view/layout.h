#ifndef HAND_VIEW_LAYOUT_H
#define HAND_VIEW_LAYOUT_H

#include "view/field.h"


class Layout : public Field::Item
{
public:
    virtual ~Layout();


    Field* GetField(const std::string& name, bool create = true) override;
    void SetField(Field* field) { m_Fields.push_back(field); }

    bool IsValid() const;
    bool IsExpanding(Orientation direction) override;

    /// Returns the vector size.
    unsigned GetValidFields(std::vector<Field*>& out);

    void Exit() override { delete this; }

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

    SDL_Rect ComputeSize(const SDL_Rect& outer) override;
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
    SDL_Rect GetCompoundSize(const std::vector<Field*>& fields);
    uint16_t SetSameSize(const std::vector<Field*>& fields, Orientation orientation);

    Orientation m_Orientation = Horizontal;
    unsigned m_MaxItemsToShow = 5;

    Expansion m_ExpansionMode = Compact;
};


Field* MakeField(Layout* in);
Field* MakeField(const std::string& in);
Layout* Split(Field* field1, Field* field2, Orientation orientation);

template<class T1, class T2>
Layout* SplitV(T1 field1, T2 field2) {
    return Split(MakeField(field1), MakeField(field2), Vertical);
}

template<class T1, class T2>
Layout* SplitH(T1* field1, T2* field2) {
    return Split(MakeField(field1), MakeField(field2), Horizontal);
}

}
#endif //HAND_VIEW_LAYOUT_H
