#ifndef HAND_VIEW_LAYER_LAYER_H
#define HAND_VIEW_LAYER_LAYER_H

#include <SDL/SDL.h> // TODO: remove SDL dependency here
#include <vector>
#include "view/drawer.h"
#include "view/layout.h"


class Drawer;
class HmiItem;
class Theme;

class Layer
{
public:
    virtual ~Layer() { Collapse(); }

    // Checks and updates content and triggers a re-draw if needed
    bool Show(bool forced);
    bool ShowChildren(bool forced);

    // Methods to (re-)set links to external objects:
    void SetParent(Layer* parent) { m_Parent = parent; }
    // TODO: any use for this?
    Layer* GetParent() { return m_Parent; }

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
    virtual void SetSize(SDL_Rect size);
    SDL_Rect GetSize() { return CoordinatesOnBuffer; }

protected:
    Layer* Insert(Layer* child);

    virtual void Exit(HmiItem*);

    virtual void Update() = 0;
    virtual Layout* CreateLayout() { return new Layout(); }
    virtual Drawer* CreatetDrawer() = 0;

    Layer* m_Parent = nullptr;
    std::vector<Layer*> m_Sublayers;

    Theme* m_Theme = nullptr;
    Drawer* m_Drawer = nullptr;

    SDL_Rect CoordinatesOnBuffer = { 0, 0, 0, 0 };

    HmiItem* m_Data = nullptr;

    bool IsVisible = true;
    bool IsExpanded = false;

    bool Changed = true;

private:
    Layout* m_Layout = nullptr;
};

#endif // HAND_VIEW_LAYER_LAYER_H
