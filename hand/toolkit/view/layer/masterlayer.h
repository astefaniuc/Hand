#ifndef HAND_VIEW_LAYER_MASTERLAYER_H
#define HAND_VIEW_LAYER_MASTERLAYER_H

#include "graph/interface.h"
#include "graph/method.h"


/// The MasterLayer is a composed Layer, managing the basic layout and theme.
/// For the sub-layers, the main command list is mandatory; other layers may be
/// additional (system) command lists, a title, a main view, and a secondary view e.g.
/// an info box or help.
class MasterLayer : public Layer
{
public:
    MasterLayer();

protected:
    void Rebuild() override;
    Drawer* CreatetDrawer() override;

private:
    void Exit(HmiItem*);
    void Clear();

    Layer* m_View = nullptr;
    Layer* m_Controls = nullptr;
    Layer* m_Auxilliary = nullptr;

    Action<MasterLayer>* m_Exit;
};

#endif // HAND_VIEW_LAYER_MASTERLAYER_H
