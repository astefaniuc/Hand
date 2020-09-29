#ifndef HAND_VIEW_LAYER_LAYER_H
#define HAND_VIEW_LAYER_LAYER_H

#include "view/drawer.h"
#include "view/layout.h"
#include <SDL/SDL.h> // TODO: remove SDL dependency here
#include <map>


class HmiItem;
class Theme;

class Layer
{
public:
    virtual ~Layer();

    // Checks and updates content and triggers a re-draw if needed
    bool Update();
    void Draw(SDL_Surface* buffer, bool forced);
    virtual void DrawChildren(SDL_Surface* buffer, bool forced) = 0;

    // Methods to (re-)set links to external objects:
    void SetParent(Layer* parent) { m_Parent = parent; }
    // TODO: any use for this?
    Layer* GetParent() const { return m_Parent; }

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

    /// Set size and position relative to the parent layer.
    void SetPreferredSize(const SDL_Rect& size);
    const SDL_Rect& GetPreferredSize() const { return m_PreferredSize; }

    void SetContentSize(const SDL_Rect& size) { m_ContentSize = size; }
    const SDL_Rect& GetContentSize() const { return m_ContentSize; }

    /// Set the available space and absolute position for drawing.
    void SetFieldSize(const SDL_Rect& outer);
    const SDL_Rect& GetFieldSize() const { return m_FieldSize; }

    SDL_Rect UpdateSize(const SDL_Rect& offset);

    virtual SDL_Rect GetLayoutSize() = 0;
    virtual void SetLayoutSize(const SDL_Rect& outer) = 0;

    bool IsModified() { return (m_IsModified || m_ModifiedContent); }
    virtual void Exit(HmiItem*);

protected:
    virtual Layout::Node* CreateLayout() = 0;
    virtual Drawer* CreatetDrawer() = 0;
    /// Rebuild sub-layer structure on content or layout changes.
    virtual void Rebuild() = 0;
    /// Update all sub-layers sizes.
    virtual void UpdateSubContent() = 0;

    Layer* m_Parent = nullptr;
    Theme* m_Theme = nullptr;
    Drawer* m_Drawer = nullptr;

    /// The layers rectangle including the frame and spacing around it, and its position.
    /// This is calculated in the UpdateSize() method and used in the parent layer size calculation.
    SDL_Rect m_PreferredSize = { 0, 0, 0, 0 };
    /// The layers content/active rectangle and its position.
    SDL_Rect m_ContentSize = { 0, 0, 0, 0 };
    /// The total available size.
    SDL_Rect m_FieldSize = { 0, 0, 0, 0 };

    HmiItem* m_Data = nullptr;

    bool IsVisible = true;
    bool IsExpanded = false;

    bool m_ModifiedContent = false;
    bool m_IsModified = false;

private:
    Layout::Node* m_Layout = nullptr;
};


class LayerMap : public Layer
{
public:
    ~LayerMap();

    void DrawChildren(SDL_Surface* buffer, bool forced) override;
    /// Returns 'sub'.
    Layer* Insert(const std::string& field, Layer* sub);
    void Remove(Layer* sub) override;

    SDL_Rect GetLayoutSize() override {
        return GetMap()->GetFieldSize(this, { 0, 0, 0, 0 });
    }

    void SetLayoutSize(const SDL_Rect& outer) override;
    Layout::MapNode* GetMap() { return static_cast<Layout::MapNode*>(GetLayout()); }
    Layer* GetField(const std::string& name);

    void UpdateSubContent() override;

protected:
    std::map<std::string, Layer*> m_Sublayers;
};


#endif // HAND_VIEW_LAYER_LAYER_H
