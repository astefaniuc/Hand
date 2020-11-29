#ifndef HAND_VIEW_LAYER_MASTERLAYER_H
#define HAND_VIEW_LAYER_MASTERLAYER_H

#include "data/interface.h"
#include "data/method.h"


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
    Layout* CreateLayout() override { return Layouts::Compact::CreateView(); }

//    void SetFocus(to or not to ...?);
//    void Keep(enum mode)

private:
    Action<View>* m_Exit;
};

}
#endif // HAND_VIEW_LAYER_MASTERLAYER_H
