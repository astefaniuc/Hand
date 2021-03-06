#ifndef HAND_VIEW_LAYER_H
#define HAND_VIEW_LAYER_H

#include "view/drawer.h"
#include "view/field.h"
#include <SDL2/SDL.h> // TODO: remove SDL dependency here


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

    void Draw(SDL_Surface* buffer) final;
    virtual void DrawContent(SDL_Surface* buffer) = 0;

    // Set pointer to a data tree node
    virtual void SetData(Hmi::Item* data);
    Hmi::Item* GetData() const { return m_Data; }
    virtual void RemoveData();

    void SetTheme(Theme* theme);
    Theme* GetTheme();

    Drawer* GetDrawer();
    void SetDrawer(Drawer* drawer);
    /// Remove Drawer without calling Drawer::Exit().
    void RemoveDrawer() { m_Drawer = nullptr; }

    Layers::List* GetListLayer() override { return nullptr; }

    const SDL_Rect& GetSize() const { return m_Size; }

    bool IsVisible() { return (m_Field && m_Field->IsVisible()); }

    void Exit() final { Quit(nullptr); }
    void Prune() override;
    void Clear() override { delete this; }
    virtual void ClearContent() {}

    virtual Hmi::List* GetLayerControls() { return nullptr; }
    virtual Layers::Interface* GetInterface();

    virtual void Activate() {}

    /// Callbacks.
    virtual void OnDataChanged(Hmi::Item*) { Update(); }
    void OnDataExit(Hmi::Item*);


    Listeners<Layer> ExitListeners;
    Listeners<Layer> DataListeners;

protected:
    void GetActiveLayer(std::vector<Layer*>& out) { out.push_back(this); }

    /// Rebuild sub-layer structure on content or layout changes.
    void Update();
    virtual void Rebuild() = 0;

    virtual Drawer* GetDrawerFromTheme() = 0;

    /// The Field::Item implementation:
    Field* GetField(const std::string& name, bool create = true) final { return nullptr; }

    SDL_Rect ComputeSize(const SDL_Rect& outer) override;
    void UpdatePositions(const SDL_Rect& outer) override;
    bool IsExpanding(Orientation direction) override { return false; }
    bool IsValid() const final { return m_IsValid; }

    virtual void Quit(Layers::Item*);

    Theme* m_Theme = nullptr;
    Drawer* m_Drawer = nullptr;

    /// The layers content/active rectangle and its position.
    SDL_Rect m_Size = { 0, 0, 0, 0 };

    Hmi::Item* m_Data = nullptr;
    bool m_IsValid = true;
};


#endif // HAND_VIEW_LAYER_H
