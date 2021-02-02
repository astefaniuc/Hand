#ifndef HAND_VIEW_LAYER_MASTERLAYER_H
#define HAND_VIEW_LAYER_MASTERLAYER_H

#include "view/layers/map.h"


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

    void AddOnExit(ICallback* cb);
    void RemoveOnExit(ICallback* cb);

    void SetInteractionControl(Interaction::Control* hand);
    void RemoveInteractionControl();

    void GetActiveItems(std::vector<Hmi::Item*>& out) override;

protected:
    void SetContent(Hmi::Item* data) override;
    void Rebuild() override;

    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override;

//    void Keep(enum mode)
    void Quit(Hmi::Item* caller) override;

private:
    ListView* m_Controls;
    Interaction::Control* m_InteractionControl = nullptr;
};

}
#endif // HAND_VIEW_LAYER_MASTERLAYER_H
