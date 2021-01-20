#ifndef HAND_VIEW_LAYERS_LIST_H
#define HAND_VIEW_LAYERS_LIST_H

#include "view/layer.h"
#include "view/layout.h"


namespace Layers {


class List : public Layer
{
public:
    ~List() { delete m_Layout; }

    Layout* GetLayout() override;
    void SetLayout(Layout* layout);

    Layers::List* GetLayer() override { return this; }

    void Update() override;

    bool UpdateFocus() override;
    void ClearFocus() override;

    void DrawContent(SDL_Surface* buffer) override { GetLayout()->Draw(buffer); }

protected:
    virtual Layout* CreateLayout() = 0;

    /// Field::Item interface:
    SDL_Rect ComputeSize(const SDL_Rect& outer) override;
    void UpdatePositions(const SDL_Rect& outer) override;
    bool IsExpanding(Orientation direction) override {
        return GetLayout()->IsExpanding(direction);
    }

    void Clear() override;

private:
    Layout* m_Layout = nullptr;
};

}
#endif // HAND_VIEW_LAYERS_LIST_H
