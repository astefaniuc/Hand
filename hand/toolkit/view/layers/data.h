#ifndef HAND_VIEW_LAYER_DATA_H
#define HAND_VIEW_LAYER_DATA_H

#include "view/layers/map.h"
#include "view/layouts/builtin.h"


namespace Layers {


class Data : public Map
{
protected:
    void SetContent(Hmi::Item* data) override;

    void Rebuild() override;

    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return Layouts::CreateData(); }

    /// Callback.
    void NotifyChanged(Hmi::Item*) { SetModifiedContent(); }

    Text m_DataText;
};

}
#endif // HAND_VIEW_LAYER_DATA_H
