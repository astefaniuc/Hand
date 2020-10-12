#ifndef HAND_VIEW_LAYERS_LIST_H
#define HAND_VIEW_LAYERS_LIST_H

#include "view/layer.h"
#include "view/layouts/compact.h"
#include <vector>


namespace Layers {


class List : public Layer
{
public:
    ~List();

    /// Returns 'sub'.
    Layer* Insert(Layer* sub);
    void Remove(Layer* sub) override;

    Layer* GetFirstChild() override;
    Layer* GetNextChild() override;

protected:
    void Rebuild() override;

    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return new Layouts::CompactList(); }

    bool SetFocus(HmiItem*);
    // Returns the list which should be mapped to the InputState
    HmiItem* GetControlsList();

    Layouts::List* GetListLayout() { return static_cast<Layouts::List*>(GetLayout()); }

private:
    std::vector<Layer*> m_Sublayers;
    std::vector<Layer*>::const_iterator m_CurrentChild = m_Sublayers.cbegin();
    unsigned m_StartPosition = 0;
};

}
#endif // HAND_VIEW_LAYERS_LIST_H
