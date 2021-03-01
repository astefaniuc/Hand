#ifndef HAND_VIEW_LAYERS_LIST_H
#define HAND_VIEW_LAYERS_LIST_H

#include "view/layer.h"
#include "view/layout.h"


namespace Layers {


class List : public Layer
{
public:
    ~List();

    Layout* GetLayout();
    void SetLayout(Layout* layout);

    Layers::List* GetListLayer() override { return this; }

    void Update() override;
    void DrawContent(SDL_Surface* buffer) override { GetLayout()->Draw(buffer); }


    bool SetInteraction(Interaction::Group* focus) override;
    void ReleaseInteractionGroup() override;

    Hmi::List* GetLayerControls() override;
    virtual void GetActiveItems(std::vector<Layer*>& out) { GetLayout()->GetActiveLayer(out); }

protected:
    virtual Layout* CreateLayout() = 0;

    void UpdateInteractionGroup() override;
    void ClearInteractionGroup() override;

    /// Field::Item interface:
    SDL_Rect ComputeSize(const SDL_Rect& outer) override;
    void UpdatePositions(const SDL_Rect& outer) override;
    bool IsExpanding(Orientation direction) override;

    void ClearContent() override;

    Layout* m_Layout = nullptr;
    Hmi::List* m_LayerCommands = nullptr;

    Interaction::Group* m_InteractionGroup = nullptr;
};

}
#endif // HAND_VIEW_LAYERS_LIST_H
