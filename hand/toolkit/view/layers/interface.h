#ifndef HAND_VIEW_LAYER_MASTERLAYER_H
#define HAND_VIEW_LAYER_MASTERLAYER_H

#include "view/layers/map.h"
#include "data/interface.h"


namespace Hmi { class Vector; }

namespace Layers {

class ListView;

/// The Interface is a composed Layer, managing the basic layout and theme.
/// For the sub-layers, the main command list is mandatory; other layers may be
/// additional (system) command lists, a title, a main view, and a secondary view e.g.
/// an info box or help.
class Interface : public Map
{
public:
    Interface();
    ~Interface();

    void Update() override;

    Interface* GetInterface() override { return this; }

    Hmi::Vector* GetView() { return m_Data->GetInterface()->GetView(); }
    void AddView(Hmi::Item* item);
    void AttachView(Hmi::Item* item);

    void SetInteractionControl(Interaction::Control* hand);
    void RemoveInteractionControl();

    void GetActiveItems(std::vector<Hmi::Item*>& out) override;

    void CollectShortcuts();

    Listeners<Interface> ExitListeners;

protected:
    void SetData(Hmi::Item* data) override;
    void Rebuild() override;

    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override;

//    void Keep(enum mode)
    void Quit(Hmi::Item* caller) override;

private:
    void SetViewRemoveCallback(Hmi::Item* item);
    void AddToRemoveFromView(Interface* item) { m_ToRemoveFromView.push_back(item); }

    ListView* m_Controls;
    Interaction::Control* m_InteractionControl = nullptr;
    Hmi::Vector* m_Shortcuts = nullptr;
    std::vector<Interface*> m_ToRemoveFromView;
};

}
#endif // HAND_VIEW_LAYER_MASTERLAYER_H
