#ifndef HAND_VIEW_LAYERS_LIST_H
#define HAND_VIEW_LAYERS_LIST_H

#include "view/layer.h"
#include "view/layout.h"


namespace Layers {


class List : public Layer
{
public:
    ~List() { delete m_Layout; }

    bool Update() override;
    /// Rebuild sub-layer structure on content or layout changes.
    virtual void Rebuild() = 0;

    Layout* GetLayout();
    void SetLayout(Layout* layout);

    void SetFocus(Hand* hand) override;
    void ReleaseFocus(Hand* hand) override;

    virtual unsigned GetChildCount() const = 0;
    virtual Layer* GetFirstChild() = 0;
    virtual Layer* GetNextChild() = 0;
    virtual Layer* GetChild(const std::string& name) const = 0;

    virtual void Remove(Layer* sub) = 0;

protected:
    virtual Layout* CreateLayout() = 0;

    /// Field::Item interface:
    SDL_Rect ComputeSize(const SDL_Rect& outer) override;
    void UpdatePositions(const SDL_Rect& outer) override;
    bool IsExpanding(Orientation direction) override {
        return GetLayout()->IsExpanding(direction);
    }
    void Quit(Hmi::Item*) override;

private:
    Layout* m_Layout = nullptr;
};

}
#endif // HAND_VIEW_LAYERS_LIST_H
