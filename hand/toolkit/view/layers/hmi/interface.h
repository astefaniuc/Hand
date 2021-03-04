#ifndef HAND_VIEW_LAYER_HMI_INTERFACE_H
#define HAND_VIEW_LAYER_HMI_INTERFACE_H

#include "view/layers/hmi/item.h"
#include "data/interface.h"


namespace Hmi { class Vector; }

namespace Layers {

class ListView;
class Vector;


class Interface : public Item
{
public:
    Interface();

    void SetData(Hmi::Item* data) override;

    Interface* GetInterface() override { return this; }

    void Show(Layer* item);

    virtual void GetInteractionGroups(Interaction::Control* hand);

    void CollectShortcuts();

    Listeners<Layer> ShowListeners;

protected:
    Hmi::Vector* GetView() { return m_Data->GetInterface()->GetView(); }
    void Rebuild() override;

    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override;

//    void Keep(enum mode)

private:
    ListView* m_Controls;
    Vector* m_View = nullptr;
    Hmi::Vector* m_Shortcuts = nullptr;
};

}
#endif // HAND_VIEW_LAYER_HMI_INTERFACE_H
