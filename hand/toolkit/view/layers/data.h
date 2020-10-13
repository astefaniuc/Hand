#ifndef HAND_VIEW_LAYER_DATA_H
#define HAND_VIEW_LAYER_DATA_H

#include "view/layers/map.h"
#include "view/layouts/compact.h"


namespace Layers {


class DataLayer : public Map
{
public:
    ~DataLayer() { Clear(); }

protected:
    void SetContent(HmiItem* data) override;

    void Rebuild() override;

    void Clear();
    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return Layouts::Compact::CreateData(); }

    /// Callback.
    void NotifyChanged(HmiItem*) { m_ModifiedContent = true; }

    Layer* m_Name = nullptr;
    Layer* m_DataText = nullptr;
    Layer* m_Description = nullptr;
};

}
#endif // HAND_VIEW_LAYER_DATA_H
