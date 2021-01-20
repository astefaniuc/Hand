#ifndef HAND_VIEW_LAYERS_LISTVIEW_H
#define HAND_VIEW_LAYERS_LISTVIEW_H

#include "view/layers/map.h"
#include "view/layers/vector.h"
#include "view/layout.h"


namespace Layers {


class ListView : public Map
{
public:
    ListView();
    ~ListView() { delete m_LayerCommands; }

    void SetContent(Hmi::Item* data) override;

    bool UpdateFocus() override;
    void ClearFocus() override;

protected:
    void Rebuild() override;

    Drawer* CreatetDrawer() override;
    Layout* CreateLayout() override;

    void Back(Hmi::Item*);

private:
    Hmi::List* m_LayerCommands = nullptr;
    Hmi::Item* m_Back = nullptr;
    Vector m_DataControls;
    std::vector<Hmi::Item*> m_ViewStack;
};

}
#endif // HAND_VIEW_LAYERS_LISTVIEW_H
