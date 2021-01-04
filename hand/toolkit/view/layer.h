#ifndef HAND_VIEW_LAYER_H
#define HAND_VIEW_LAYER_H

#include "view/drawer.h"
#include "input/chord.h"
#include "view/field.h"
#include <SDL/SDL.h> // TODO: remove SDL dependency here


namespace Hmi { class Item; }
namespace Layers { class List; }
class Theme;
class Hand;

class Layer : public Field::Item
{
public:
    virtual ~Layer() { delete m_Drawer; }

    // Checks and updates content and triggers a re-draw if needed
    virtual bool Update() = 0;
    void Draw(SDL_Surface* buffer);

    // Methods to (re-)set links to external objects:
    void SetParent(Layers::List* parent) { m_Parent = parent; }
    // TODO: any use for this?
    Layers::List* GetParent() const { return m_Parent; }

    // Set pointer to a data tree node
    virtual void SetContent(Hmi::Item* data);
    Hmi::Item* GetContent() const { return m_Data; }

    void SetTheme(Theme* theme);
    Theme* GetTheme();

    Drawer* GetDrawer();
    void SetDrawer(Drawer* drawer);

    const SDL_Rect& GetSize() const { return m_Size; }

    bool IsModified() { return (m_IsModified || m_ModifiedContent); }
    virtual void Exit(Hmi::Item*);

    virtual void SetFocus(Hand* hand);

protected:
    virtual Drawer* CreatetDrawer() = 0;

    /// The Field::Item implementation:
    Field* GetField(const std::string& name, bool create = true) final { return nullptr; }
    SDL_Rect ComputeSize(const SDL_Rect& outer) override;
    void UpdatePositions(const SDL_Rect& outer) override;
    bool IsExpanding(Orientation direction) override { return false; }
    bool IsValid() const final { return true; }
    void Exit() final { Exit(nullptr); }


    Layers::List* m_Parent = nullptr;
    Theme* m_Theme = nullptr;
    Drawer* m_Drawer = nullptr;

    /// The layers content/active rectangle and its position.
    SDL_Rect m_Size = { 0, 0, 0, 0 };

    Hmi::Item* m_Data = nullptr;

    bool m_ModifiedContent = false;
    bool m_IsModified = true;
};


#endif // HAND_VIEW_LAYER_H
