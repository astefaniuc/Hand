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
    SDL_Rect UpdateSize(const SDL_Rect& outer) final;
    void UpdatePositions(const SDL_Rect& outer) final;
    /// Rebuild sub-layer structure on content or layout changes.
    virtual void Rebuild() = 0;

    Layout* GetLayout();
    void SetLayout(Layout* layout);

    virtual unsigned GetChildCount() const = 0;
    virtual Layer* GetFirstChild() = 0;
    virtual Layer* GetNextChild() = 0;
    virtual Layer* GetChild(const std::string& name) const = 0;

    virtual void Remove(Layer* sub) = 0;

protected:
    virtual Layout* CreateLayout() = 0;

private:
    Layout* m_Layout = nullptr;
};

}
#endif // HAND_VIEW_LAYERS_LIST_H
