#ifndef HAND_VIEW_LAYER_LAYER_H
#define HAND_VIEW_LAYER_LAYER_H

#include "view/drawer.h"
#include "view/layout.h"
#include <SDL/SDL.h> // TODO: remove SDL dependency here
#include <vector>


class Drawer;
class HmiItem;
class Theme;

class Layer
{
public:
    virtual ~Layer();

    // Checks and updates content and triggers a re-draw if needed
    bool Update();
    void Draw(bool forced);

    // Methods to (re-)set links to external objects:
    void SetParent(Layer* parent) { m_Parent = parent; }
    // TODO: any use for this?
    Layer* GetParent() { return m_Parent; }

    /// Returns 'sub'.
    Layer* Insert(Layer* sub);
    void Remove(Layer* sub);

    const std::vector<Layer*>& GetSubLayers() const { return m_Sublayers; }

    // Set pointer to a data tree node
    void SetContent(HmiItem* data);
    HmiItem* GetContent() const { return m_Data; }

    Layout* GetLayout();
    void SetLayout(Layout* layout);

    void SetTheme(Theme* theme);
    Theme* GetTheme();

    Drawer* GetDrawer();
    void SetDrawer(Drawer* drawer);

    void Collapse();

    // Set coordinates and size relative to the parent layer
    void SetSize(const SDL_Rect& size);
    const SDL_Rect& GetSize() { return m_Coordinates; }

    /// Returns 'sub'.
    void SetSubSize(Layer* sub, const std::string& field);

    virtual void Exit(HmiItem*);

protected:
    void SetSubSize(Layer* sub, const Rel_Rect& fieldSize);

    virtual Layout* CreateLayout() = 0;
    virtual Drawer* CreatetDrawer() = 0;
    /// Rebuild sub-layer structure on content or layout changes.
    virtual void Rebuild() = 0;
    /// Update all sub-layers sizes.
    virtual void UpdateSubSizes() = 0;

    Layer* m_Parent = nullptr;
    std::vector<Layer*> m_Sublayers;

    Theme* m_Theme = nullptr;
    Drawer* m_Drawer = nullptr;

    SDL_Rect m_Coordinates = { 0, 0, 0, 0 };

    HmiItem* m_Data = nullptr;

    bool IsVisible = true;
    bool IsExpanded = false;

    bool m_ChangedContent = false;
    bool m_IsChanged = false;

private:
    Layout* m_Layout = nullptr;
};

#endif // HAND_VIEW_LAYER_LAYER_H
