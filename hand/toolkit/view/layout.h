#ifndef HAND_VIEW_LAYOUT_H
#define HAND_VIEW_LAYOUT_H

#include "view/datatypes/rect.h"


class Layout
{
public:
    virtual ~Layout() {}

    const Rel_Rect& GetCoordinates() const { return m_Coordinates; }
    void SetCoordinates(const Rel_Rect& value) { m_Coordinates = value; }

    bool IsFramed() { return m_IsFramed; }
    void SetFramed(bool enabled) { m_IsFramed = enabled; }

private:
    Rel_Rect m_Coordinates;
    bool m_IsFramed = true;
};


class ListLayout : public Layout
{
public:
    const Rel_Rect& GetAlignment() const { return m_Alignment; }
    void SetAlignment(const Rel_Rect& value) { m_Alignment = value; }

private:
    Rel_Rect m_Alignment;
};

#endif //HAND_VIEW_LAYOUT_H
