#ifndef HAND_VIEW_LAYER_MASTERLAYER_H
#define HAND_VIEW_LAYER_MASTERLAYER_H

#include "view/layers/map.h"


namespace Layers {


/// The View is a composed Layer, managing the basic layout and theme.
/// For the sub-layers, the main command list is mandatory; other layers may be
/// additional (system) command lists, a title, a main view, and a secondary view e.g.
/// an info box or help.
class View : public Map
{
public:
    View();

protected:
    void Rebuild() override;

    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override;

    void SetFocus(Hand* hand) override;
//    void Keep(enum mode)
    void Quit(Hmi::Item*) override;

private:
    Hmi::List* m_LayerCommands;
    Hand* m_Hand = nullptr;
};

}
#endif // HAND_VIEW_LAYER_MASTERLAYER_H
