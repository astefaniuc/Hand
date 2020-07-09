#ifndef HAND_VIEW_LAYER_LAYER_H
#define HAND_VIEW_LAYER_LAYER_H

#include "view/layer/virtualsurface.h"


class Layer : public VirtualSurface
{
public:
    virtual ~Layer() { Collapse(); }
    Layer() : m_FrameColor("FrameColor"), m_BackgroundColor("FrameColor") {}

    // Checks and updates content and triggers a re-draw if needed
    bool Update(bool force);

    // Methods to (re-)set links to external objects:
    void SetParentLayer(Layer* parent) { ParentLayer = parent; }
    // TODO: any use for this?
    Layer* GetParentLayer() { return ParentLayer; }


    // Set pointer to a data tree node
    virtual void SetContent(HmiItem* data);
    HmiItem* GetContent() { return m_Data; }

    void Collapse();

    virtual void NotifyChanged() { Changed = true; }

    virtual void Draw(bool forced);

    const Rgb& GetFrameColor() { return m_FrameColor; }
    void SetFrameColor(const Rgb& color) { m_FrameColor = color; }

    const Rgb& GetBackgroundColor() { return m_BackgroundColor; }
    void SetBackgroundColor(const Rgb& color) { m_BackgroundColor = color; }

protected:
    virtual void DrawFrame();
    // Insert "data" as layer of type layer_type at "position"
    Layer* Insert(Layer* child);

    virtual void DrawChildren(bool forced);

    virtual void Exit(HmiItem*);

    Layer* ParentLayer = nullptr;
    std::vector<Layer*> m_Sublayers;
    VirtualSurface* m_ScreenRect = nullptr;

//        bool Updated;
public:
    HmiItem* m_Data = nullptr;
    Rgb m_FrameColor;
    Rgb m_BackgroundColor;

    bool IsVisible = true;
    bool IsExpanded = false;
};

#endif // HAND_VIEW_LAYER_LAYER_H
