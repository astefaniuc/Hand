#ifndef HAND_VIEW_LAYER_H
#define HAND_VIEW_LAYER_H

#include "view/drawer.h"
#include "view/layout.h"
#include "input/chord.h"
#include <SDL/SDL.h> // TODO: remove SDL dependency here


class HmiItem;
class Theme;

class Layer
{
public:
    virtual ~Layer();

    // Checks and updates content and triggers a re-draw if needed
    bool Update();
    void Draw(SDL_Surface* buffer);

    // Methods to (re-)set links to external objects:
    void SetParent(Layer* parent) { m_Parent = parent; }
    // TODO: any use for this?
    Layer* GetParent() const { return m_Parent; }

    virtual unsigned GetChildCount() const { return 0; };
    virtual Layer* GetFirstChild() { return nullptr; }
    virtual Layer* GetNextChild() { return nullptr; }
    virtual Layer* GetChild(const std::string& name) const { return nullptr; }

    virtual void Remove(Layer* sub) = 0;

    // Set pointer to a data tree node
    virtual void SetContent(HmiItem* data);
    HmiItem* GetContent() const { return m_Data; }

    Layout* GetLayout();
    void SetLayout(Layout* layout);

    void SetTheme(Theme* theme);
    Theme* GetTheme();

    Drawer* GetDrawer();
    void SetDrawer(Drawer* drawer);

    /// Returns the layers rectangle including the frame and spacing around it.
    SDL_Rect UpdateSize(const SDL_Rect& outer);
    /// Returns the size without the frame and spacing.
    const SDL_Rect& GetContentSize() const { return m_Size; }

    bool IsModified() { return (m_IsModified || m_ModifiedContent); }
    virtual void Exit(HmiItem*);

protected:
    virtual Layout* CreateLayout() = 0;
    virtual Drawer* CreatetDrawer() = 0;
    /// Rebuild sub-layer structure on content or layout changes.
    virtual void Rebuild() = 0;
    /// Update all sub-layers sizes.

    Layer* m_Parent = nullptr;
    Theme* m_Theme = nullptr;
    Drawer* m_Drawer = nullptr;

    /// The layers content/active rectangle and its position.
    SDL_Rect m_Size = { 0, 0, 0, 0 };

    HmiItem* m_Data = nullptr;

    bool m_ModifiedContent = false;
    bool m_IsModified = true;

private:
    Layout* m_Layout = nullptr;
};


#endif // HAND_VIEW_LAYER_H
