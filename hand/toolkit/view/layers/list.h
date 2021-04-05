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

    void Insert(Layer* sub) { Insert(sub->GetData()->GetName(), sub); }
    void Insert(const std::string& field, Layer* sub)
    {
        GetLayout()->GetField(field)->SetItem(sub);
    }

    void Prune() override;

    void DrawContent(SDL_Surface* buffer) override { GetLayout()->Draw(buffer); }

    Layers::List* GetListLayer() override { return this; }

    Hmi::List* GetLayerControls() override;
    virtual void GetActiveItems(std::vector<Layer*>& out) { GetLayout()->GetActiveLayer(out); }

    void SetExpandChildren(bool expand);

protected:
    virtual Layout* CreateLayout() = 0;

    /// Field::Item interface:
    SDL_Rect ComputeSize(const SDL_Rect& outer) override;
    void UpdatePositions(const SDL_Rect& outer) override;
    bool IsExpanding(Orientation direction) override { return GetLayout()->IsExpanding(direction); }

    void ClearContent() override { GetLayout()->Clear(); }

    Hmi::List* m_LayerCommands = nullptr;
    bool m_ExpandChildren = false;

private:
    Layout* m_Layout = nullptr;
};

}
#endif // HAND_VIEW_LAYERS_LIST_H
