#ifndef HAND_VIEW_LAYERS_LISTVIEW_H
#define HAND_VIEW_LAYERS_LISTVIEW_H

#include "view/layers/map.h"
#include "view/layers/vector.h"
#include "view/layout.h"


namespace Layers {


class ListView : public Map
{
public:
    void SetData(Hmi::Item* data) override;

protected:
    void Rebuild() override;

    Drawer* GetDrawerFromTheme() override;
    Layout* CreateLayout() override;

    void GetActiveItems(std::vector<Hmi::Item*>& out) override
    {
        m_DataControls.GetActiveItems(out);
    }


    void Back(Hmi::Item*);

private:
    Hmi::Item* m_Back = nullptr;
    Vector m_DataControls;
    std::vector<Hmi::Item*> m_ViewStack;
};

}
#endif // HAND_VIEW_LAYERS_LISTVIEW_H
