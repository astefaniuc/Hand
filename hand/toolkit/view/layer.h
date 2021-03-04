#ifndef HAND_VIEW_LAYER_H
#define HAND_VIEW_LAYER_H

#include "view/drawer.h"
#include "view/field.h"
#include <SDL/SDL.h> // TODO: remove SDL dependency here


namespace Layers {
    class Item;
    class List;
    class Interface;
}
class Theme;

class Layer : public Field::Item
{
public:
    virtual ~Layer();

    // Checks and updates content and triggers a re-draw if needed
    virtual void Update();
    void Draw(SDL_Surface* buffer) final;
    virtual void DrawContent(SDL_Surface* buffer) = 0;

    // Set pointer to a data tree node
    virtual void SetData(Hmi::Item* data);
    Hmi::Item* GetData() const { return m_Data; }

    void SetTheme(Theme* theme);
    Theme* GetTheme();

    Drawer* GetDrawer();
    void SetDrawer(Drawer* drawer);
    /// Remove Drawer without calling Drawer::Exit().
    void RemoveDrawer() { m_Drawer = nullptr; }

    Layers::List* GetListLayer() override { return nullptr; }

    const SDL_Rect& GetSize() const { return m_Size; }

    bool IsModified() { return (m_IsModified || m_ModifiedContent); }
    void SetModified();

    bool IsVisible() { return (m_Field && m_Field->IsVisible()); }

    void Exit() final { Quit(nullptr); }
    void Clear() override;
    virtual void ClearContent() {}

    virtual Hmi::List* GetLayerControls() { return nullptr; }
    virtual Layers::Interface* GetInterface();

    virtual void Activate() {}


    Listeners<Layer> ExitListeners;

protected:
    virtual bool CanUpdate() { return true; }

    void GetActiveLayer(std::vector<Layer*>& out) { out.push_back(this); }

    void SetModifiedContent();
    /// Rebuild sub-layer structure on content or layout changes.
    virtual void Rebuild() = 0;

    virtual Drawer* GetDrawerFromTheme() = 0;

    /// The Field::Item implementation:
    Field* GetField(const std::string& name, bool create = true) final { return nullptr; }

    SDL_Rect ComputeSize(const SDL_Rect& outer) override;
    void UpdatePositions(const SDL_Rect& outer) override;
    bool IsExpanding(Orientation direction) override { return false; }
    bool IsValid() const final { return true; }
    virtual void Quit(Layers::Item*);

    void RemoveData();

    /// Callbacks.
    void OnDataChanged(Hmi::Item*) { SetModifiedContent(); }
    void OnDataExit(Hmi::Item*);

    Theme* m_Theme = nullptr;
    Drawer* m_Drawer = nullptr;

    /// The layers content/active rectangle and its position.
    SDL_Rect m_Size = { 0, 0, 0, 0 };

    Hmi::Item* m_Data = nullptr;

private:
    bool m_ModifiedContent = false;
    bool m_IsModified = true;
};


#endif // HAND_VIEW_LAYER_H
