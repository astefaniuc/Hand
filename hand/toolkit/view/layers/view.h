#ifndef HAND_VIEW_LAYER_MASTERLAYER_H
#define HAND_VIEW_LAYER_MASTERLAYER_H

#include "view/layers/map.h"


namespace Layers {

class ListView;

/// The View is a composed Layer, managing the basic layout and theme.
/// For the sub-layers, the main command list is mandatory; other layers may be
/// additional (system) command lists, a title, a main view, and a secondary view e.g.
/// an info box or help.
class View : public Map
{
public:
    View();
    ~View();

protected:
    void SetContent(Hmi::Item* data) override;
    void Rebuild() override;

    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override;

    void SetFocus(Hand* hand) override;
    void ReleaseFocus(Hand* hand) override;
//    void Keep(enum mode)
    void Quit(Hmi::Item*) override;

private:
    Hmi::List* m_LayerCommands;
    ListView* m_Controls;

    Hand* m_Hand = nullptr;
};

}
#endif // HAND_VIEW_LAYER_MASTERLAYER_H
