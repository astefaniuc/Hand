#ifndef HAND_VIEW_LAYER_HMI_ITEM_H
#define HAND_VIEW_LAYER_HMI_ITEM_H

#include "view/layers/list.h"


namespace Layers {


class Item : public List
{
public:
    void SetData(Hmi::Item* data) override;
    void RemoveData() override;

    void Activate() override { ActivationListeners.Notify(this); }

    Listeners<Layers::Item> ActivationListeners;

protected:
    void Rebuild() override;
};

}
#endif // HAND_VIEW_LAYER_HMI_ITEM_H
