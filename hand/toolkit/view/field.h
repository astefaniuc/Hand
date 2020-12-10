#ifndef HAND_VIEW_FIELD_H
#define HAND_VIEW_FIELD_H

#include "view/layout.h"
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


class Field
{
public:
    Field(const std::string& name) : m_Name(name) { delete m_Layout; }

    /// Returns the size from the matching sub-layer.
    SDL_Rect ComputeSize(const SDL_Rect& outer);
    void SetPlacedPosition(const SDL_Rect& outer);

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

    SDL_Rect Frame;

protected:
    SDL_Rect Size;
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

#endif // HAND_VIEW_FIELD_H
