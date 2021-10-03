#ifndef HAND_VIEW_LAYERS_VECTOR_H
#define HAND_VIEW_LAYERS_VECTOR_H

#include "view/layers/list.h"
#include "view/layout.h"
#include <vector>


namespace Layers {


class Vector : public List
{
public:
    unsigned GetMaxItemsToShow() { return m_MaxItemsToShow; }
    void SetMaxItemsToShow(unsigned count) { m_MaxItemsToShow = count; }

protected:
    void Rebuild() override;

    void AddLayer(Data::Item* data);

    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override { return new Layouts::Vector(); }

private:
    unsigned m_StartPosition = 0;
    unsigned m_MaxItemsToShow = 5;
};

}
#endif // HAND_VIEW_LAYERS_VECTOR_H
