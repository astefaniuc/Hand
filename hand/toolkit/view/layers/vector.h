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

protected:
    void Rebuild() override;

    void AddLayer(Hmi::Item* data);

    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return new Layouts::List(); }

    bool SetFocus(Hmi::Item*);
    // Returns the list which should be mapped to the InputState
    Hmi::Item* GetControlsList();

    Layouts::List* GetListLayout() { return static_cast<Layouts::List*>(GetLayout()); }

private:
    std::vector<Layer*> m_Sublayers;
    std::vector<Layer*>::const_iterator m_CurrentChild = m_Sublayers.cbegin();
    unsigned m_StartPosition = 0;
    bool m_ExpandChildren = false;
};

}
#endif // HAND_VIEW_LAYERS_VECTOR_H
