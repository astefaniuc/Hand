#ifndef HAND_VIEW_LAYERS_VECTOR_H
#define HAND_VIEW_LAYERS_VECTOR_H

#include "view/layers/list.h"
#include "view/layout.h"
#include <vector>


namespace Layers {


class Vector : public List
{
public:
    ~Vector();

    /// Returns 'sub'.
    Layer* Insert(Layer* sub);
    void Remove(Layer* sub) override;

    unsigned GetChildCount() const override { return m_Sublayers.size(); }
    Layer* GetFirstChild() override;
    Layer* GetNextChild() override;
    /// Finds the child Layer by its data item name.
    Layer* GetChild(const std::string& name) const override;

    void SetExpandChildren(bool expand);

    unsigned GetMaxItemsToShow() { return m_MaxItemsToShow; }
    void SetMaxItemsToShow(unsigned count) { m_MaxItemsToShow = count; }

protected:
    void Rebuild() override;

    void AddLayer(Hmi::Item* data);

    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return new Layouts::List(); }

private:
    void ClearContainer() override { m_Sublayers.clear(); }

    std::vector<Layer*> m_Sublayers;
    std::vector<Layer*>::const_iterator m_CurrentChild = m_Sublayers.cbegin();
    unsigned m_StartPosition = 0;
    unsigned m_MaxItemsToShow = 5;
    bool m_ExpandChildren = false;
};

}
#endif // HAND_VIEW_LAYERS_VECTOR_H
