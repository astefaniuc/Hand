#ifndef HAND_VIEW_LAYER_LAYER_H
#define HAND_VIEW_LAYER_LAYER_H

#include "view/drawer.h"
#include "view/layout.h"
#include <SDL/SDL.h> // TODO: remove SDL dependency here
#include <map>


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
    virtual void DrawChildren(bool forced) = 0;

    // Methods to (re-)set links to external objects:
    void SetParent(Layer* parent) { m_Parent = parent; }
    // TODO: any use for this?
    Layer* GetParent() { return m_Parent; }

    virtual void Remove(Layer* sub) = 0;

    // Set pointer to a data tree node
    virtual void SetContent(HmiItem* data);
    HmiItem* GetContent() const { return m_Data; }

    Layout::Node* GetLayout();
    void SetLayout(Layout::Node* layout);

    void SetTheme(Theme* theme);
    Theme* GetTheme();

    Drawer* GetDrawer();
    void SetDrawer(Drawer* drawer);

    void Collapse();

    // Set coordinates and size relative to the parent layer
    void SetSize(const SDL_Rect& size);
    const SDL_Rect& GetSize() { return m_Coordinates; }

    void SetContentSize(const SDL_Rect& size) { m_ContentSize = size; }
    const SDL_Rect& GetContentSize() { return m_ContentSize; }

    SDL_Rect UpdateSize(const SDL_Rect& offset);
    virtual SDL_Rect GetLayoutSize(const SDL_Rect& offset) = 0;

    virtual void Exit(HmiItem*);

protected:
    SDL_Rect GetFramedSize(SDL_Rect& content, const SDL_Rect& offset);

    virtual Layout::Node* CreateLayout() = 0;
    virtual Drawer* CreatetDrawer() = 0;
    /// Rebuild sub-layer structure on content or layout changes.
    virtual void Rebuild() = 0;
    /// Update all sub-layers sizes.
    virtual void UpdateSubContent() = 0;

    Layer* m_Parent = nullptr;
    Theme* m_Theme = nullptr;
    Drawer* m_Drawer = nullptr;

    /// Total size plus position.
    SDL_Rect m_Coordinates = { 0, 0, 0, 0 };
    SDL_Rect m_ContentSize = { 0, 0, 0, 0 };

    HmiItem* m_Data = nullptr;

    bool IsVisible = true;
    bool IsExpanded = false;

    bool m_ChangedContent = false;
    bool m_IsChanged = false;

private:
    Layout::Node* m_Layout = nullptr;
};


class LayerMap : public Layer
{
public:
    ~LayerMap();

    void DrawChildren(bool forced) override;
    /// Returns 'sub'.
    Layer* Insert(const std::string& field, Layer* sub);
    void Remove(Layer* sub) override;

    SDL_Rect GetLayoutSize(const SDL_Rect& offset) override {
        return GetMap()->GetFieldSize(this, GetDrawer()->GetFrameOffset());
    }

    Layout::MapNode* GetMap() { return static_cast<Layout::MapNode*>(GetLayout()); }
    Layer* GetField(const std::string& name);

    void UpdateSubContent() override;

protected:
    std::map<std::string, Layer*> m_Sublayers;
};


#endif // HAND_VIEW_LAYER_LAYER_H
