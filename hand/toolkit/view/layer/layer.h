#ifndef HAND_VIEW_LAYER_LAYER_H
#define HAND_VIEW_LAYER_LAYER_H

#include <SDL/SDL.h> // TODO: remove SDL dependency here
#include <vector>
#include "view/drawer.h"
#include "view/layout.h"


class Drawer;
class HmiItem;

class Layer
{
public:
    virtual ~Layer() { Collapse(); }

    // Checks and updates content and triggers a re-draw if needed
    bool Update(bool force);

    // Methods to (re-)set links to external objects:
    void SetParent(Layer* parent) { m_Parent = parent; }
    // TODO: any use for this?
    Layer* GetParent() { return m_Parent; }

    const std::vector<Layer*>& GetSubLayers() const { return m_Sublayers; }

    // Set pointer to a data tree node
    virtual void SetContent(HmiItem* data);
    HmiItem* GetContent() const { return m_Data; }

    virtual Layout* GetLayout();
    void SetLayout(Layout* layout);

    Drawer* GetDrawer() const { return m_Drawer; }
    void SetDrawer(Drawer* drawer);

    void Collapse();

    // Set coordinates and size relative to the parent layer
    virtual void SetSize(SDL_Rect size);
    SDL_Rect GetSize() { return CoordinatesOnBuffer; }

protected:
    // Insert "data" as layer of type layer_type at "position"
    Layer* Insert(Layer* child);

    virtual void Exit(HmiItem*);

    Layer* m_Parent = nullptr;
    std::vector<Layer*> m_Sublayers;
    Drawer* m_Drawer = nullptr;
    Layout* m_Layout = nullptr;

    SDL_Rect CoordinatesOnBuffer = { 0, 0, 0, 0 };

    HmiItem* m_Data = nullptr;

    bool IsVisible = true;
    bool IsExpanded = false;

    bool Changed = true;
};

#endif // HAND_VIEW_LAYER_LAYER_H
