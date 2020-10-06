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
    void Draw(SDL_Surface* buffer);

    // Methods to (re-)set links to external objects:
    void SetParent(Layer* parent) { m_Parent = parent; }
    // TODO: any use for this?
    Layer* GetParent() const { return m_Parent; }

    virtual Layer* GetFirstChild() { return nullptr; }
    virtual Layer* GetNextChild() { return nullptr; }

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

    /// Returns the layers rectangle including the frame and spacing around it.
    SDL_Rect UpdateSize(const SDL_Rect& offset);
    /// Returns the size without the frame and spacing.
    const SDL_Rect& GetContentSize() const { return m_Size; }

    bool IsModified() { return (m_IsModified || m_ModifiedContent); }
    virtual void Exit(HmiItem*);

protected:
    virtual Layout::Node* CreateLayout() = 0;
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

    bool IsVisible = true;
    bool IsExpanded = false;

    bool m_ModifiedContent = false;
    bool m_IsModified = true;

private:
    Layout::Node* m_Layout = nullptr;
};


class LayerMap : public Layer
{
public:
    ~LayerMap();

    Layer* GetFirstChild() override;
    Layer* GetNextChild() override;

    /// Returns 'sub'.
    Layer* Insert(const std::string& field, Layer* sub);
    void Remove(Layer* sub) override;

    Layer* GetField(const std::string& name);

protected:
    std::map<std::string, Layer*> m_Sublayers;
    std::map<std::string, Layer*>::const_iterator m_CurrentChild = m_Sublayers.cbegin();
};


#endif // HAND_VIEW_LAYER_LAYER_H
