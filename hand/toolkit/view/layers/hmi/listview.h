#ifndef HAND_VIEW_LAYER_HMI_LISTVIEW_H
#define HAND_VIEW_LAYER_HMI_LISTVIEW_H

#include "view/layers/hmi/item.h"
#include "view/layers/vector.h"
#include "view/layout.h"


namespace Layers {


class ListView : public Item
{
public:
    void SetData(Hmi::Item* data) override;

protected:
    void Rebuild() override;

    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override;

    void GetActiveItems(std::vector<Layer*>& out) override;

    void Back(Layers::Item*);

private:
    Hmi::Item* m_Back = nullptr;
    List* m_DataControls = nullptr;
    std::vector<Hmi::Item*> m_ViewStack;
};

}
#endif // HAND_VIEW_LAYER_HMI_LISTVIEW_H
