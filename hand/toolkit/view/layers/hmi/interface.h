#ifndef HAND_VIEW_LAYER_HMI_INTERFACE_H
#define HAND_VIEW_LAYER_HMI_INTERFACE_H

#include "view/layers/hmi/item.h"
#include "data/interface.h"


namespace Hmi { class Vector; }

namespace Layers {

class ListView;
class Vector;

/// The Interface is a composed Layer, managing the basic layout and theme.
/// For the sub-layers, the main command list is mandatory; other layers may be
/// additional (system) command lists, a title, a main view, and a secondary view e.g.
/// an info box or help.
class Interface : public Item
{
public:
    Interface();
    ~Interface();

    void SetData(Hmi::Item* data) override;

    Interface* GetInterface() override { return this; }

    void Show(Layer* item);

    void SetInteractionControl(Interaction::Control* hand);
    void RemoveInteractionControl();

    void CollectShortcuts();

    Listeners<Interface> ExitListeners;

protected:
    Hmi::Vector* GetView() { return m_Data->GetInterface()->GetView(); }
    void Rebuild() override;

    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override;

//    void Keep(enum mode)
    void Quit(Item* caller) override;

private:
    ListView* m_Controls;
    Vector* m_View = nullptr;
    Interaction::Control* m_InteractionControl = nullptr;
    Hmi::Vector* m_Shortcuts = nullptr;
};

}
#endif // HAND_VIEW_LAYER_HMI_INTERFACE_H
