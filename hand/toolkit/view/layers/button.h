#ifndef HAND_VIEW_LAYER_BUTTON_H
#define HAND_VIEW_LAYER_BUTTON_H

#include "view/layers/map.h"
#include "view/layouts/compact.h"


namespace Layers {


class Button : public Map
{
public:
    ~Button() { Clear(); }

protected:
    void Rebuild() override;

    void Clear();
    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override { return Layouts::CreateButton(); }

    Layer* m_Name = nullptr;
    Layer* m_Description = nullptr;
};

}
#endif // HAND_VIEW_LAYER_BUTTON_H
