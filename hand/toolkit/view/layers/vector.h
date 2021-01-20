#ifndef HAND_VIEW_LAYERS_VECTOR_H
#define HAND_VIEW_LAYERS_VECTOR_H

#include "view/layers/list.h"
#include "view/layout.h"
#include <vector>


namespace Layers {


class Vector : public List
{
public:
    /// Returns 'sub'.
    Layer* Insert(Layer* sub);

    void SetExpandChildren(bool expand);

    unsigned GetMaxItemsToShow() { return m_MaxItemsToShow; }
    void SetMaxItemsToShow(unsigned count) { m_MaxItemsToShow = count; }

protected:
    void Rebuild() override;

    void AddLayer(Hmi::Item* data);

    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return new Layouts::List(); }

private:
    unsigned m_StartPosition = 0;
    unsigned m_MaxItemsToShow = 5;
    bool m_ExpandChildren = false;
};

}
#endif // HAND_VIEW_LAYERS_VECTOR_H
