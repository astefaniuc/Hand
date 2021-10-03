#ifndef HAND_VIEW_LAYER_HMI_INTERFACE_H
#define HAND_VIEW_LAYER_HMI_INTERFACE_H

#include "view/layers/hmi/item.h"
#include "data/interface.h"


namespace Data { class Vector; }
namespace Interaction { class Control; }

namespace Layers
{

class ListView;
class Vector;


class Interface : public Item
{
public:
    Interface();

    Interface* GetInterface() override { return this; }

    void Show(Layer* item);

    virtual void GetInteractionGroups(Interaction::Control* hand);

    void CollectShortcuts();

    Listeners<Layer> ShowListeners;

protected:
    List* GetView() { return GetLayout()->GetField(VIEW, false)->GetItem()->GetListLayer(); }
    void Rebuild() override;

    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override;

//    void Keep(enum mode)

private:
    ListView* m_Controls;
    Data::Vector* m_Shortcuts = nullptr;
};

}
#endif // HAND_VIEW_LAYER_HMI_INTERFACE_H
