#ifndef HAND_VIEW_FIELD_H
#define HAND_VIEW_FIELD_H

#include "include/stdfields.h"
#include "view/datatypes/rect.h"


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


enum Orientation
{
    Auto,
    Horizontal,
    Vertical
};


class Layout;
class Theme;
namespace Layers { class List; }

class Field
{
public:
    class Item
    {
    public:
        virtual ~Item() = default;

        void SetParentField(Field* field);
        Field* GetParentField() { return m_Field; }

        virtual Item* GetParentFieldItem();
        Layers::List* GetParentLayer();

        virtual Layers::List* GetListLayer() = 0;

        virtual Field* GetField(const std::string& name, bool create = true) = 0;
        virtual void GetActiveLayer(std::vector<Layer*>& out) = 0;

        virtual SDL_Rect ComputeSize(const SDL_Rect& outer) = 0;
        virtual void UpdatePositions(const SDL_Rect& outer) = 0;
        virtual void Draw(SDL_Surface* buffer) = 0;

        virtual bool IsExpanding(Orientation direction) = 0;
        virtual bool IsValid() const = 0;

        virtual void Exit() = 0;
        virtual void Clear() = 0;
        virtual void Prune() = 0;

        bool IsModified() { return m_IsModified; }
        void SetModified(bool state = true);

    protected:
        Field* m_Field = nullptr;

    private:
        bool m_IsModified = true;
    };

    Field(const std::string& name) : m_Name(name) {}
    ~Field();

    const std::string& GetName() { return m_Name; }

    /// Returns the size from the matching sub-layer.
    SDL_Rect ComputeSize(const SDL_Rect& outer);
    void SetPlacedPosition(const SDL_Rect& outer);

    Field* GetField(const std::string& name) const;

    void SetItem(Item* item);
    Item* GetItem() { return m_Item; }
    void RemoveItem();

    Layout* GetParentLayout() { return m_Parent; }
    void SetParentLayout(Layout* parent) { m_Parent = parent; }

    void SetVisible(bool visible) { m_IsVisible = visible; }
    bool IsVisible() { return m_IsVisible; }

    void SetAlignment(VAlignment::Position position) { m_AlignmentV.Pos = position; }
    void SetAlignment(HAlignment::Position position) { m_AlignmentH.Pos = position; }
    void SetAlignment(VAlignment::Position vertical, HAlignment::Position horizontal);
    void Align();

    void SetPosition(const RelRect& pos) { m_Position = pos;}

    bool IsValid() const { return (m_IsVisible && m_Item && m_Item->IsValid()); }

    void SetExpanding(bool vertical, bool horizontal);
    bool IsExpanding(Orientation direction) const;

    /// External size, modified in the layout.
    SDL_Rect Frame;

private:
    // Content size and position
    SDL_Rect m_Size = { 0, 0, 0, 0 };
    Item* m_Item = nullptr;
    Layout* m_Parent = nullptr;

    bool m_IsVisible = true;
    bool m_ExpandV = false;
    bool m_ExpandH = false;
    VAlignment m_AlignmentV;
    HAlignment m_AlignmentH;
    RelRect m_Position;

    std::string m_Name;
};

#endif // HAND_VIEW_FIELD_H
