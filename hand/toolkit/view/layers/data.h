#ifndef HAND_VIEW_LAYER_DATA_H
#define HAND_VIEW_LAYER_DATA_H

#include "view/layers/map.h"
#include "view/layouts/compact.h"


namespace Layers {


class Data : public Map
{
protected:
    void SetContent(Hmi::Item* data) override;

    void Rebuild() override;

    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return Layouts::Compact::CreateData(); }

    /// Callback.
    void NotifyChanged(Hmi::Item*) { m_ModifiedContent = true; }

    Text m_DataText;
};

}
#endif // HAND_VIEW_LAYER_DATA_H
