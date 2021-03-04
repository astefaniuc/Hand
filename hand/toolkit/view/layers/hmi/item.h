#ifndef HAND_VIEW_LAYER_HMI_ITEM_H
#define HAND_VIEW_LAYER_HMI_ITEM_H

#include "view/layers/map.h"


namespace Layers {


class Item : public Map
{
public:
    void SetData(Hmi::Item* data) override;

    bool SetCommand(Interaction::Command* ctrl) override;
    Interaction::Command* GetCommand() override { return m_Command; }
    void ReleaseCommand() override;

    void Activate() override { ActivationListeners.Notify(this); }

    Listeners<Layers::Item> ActivationListeners;

protected:
    void Rebuild() override;

    Interaction::Command* m_Command = nullptr;
};

}
#endif // HAND_VIEW_LAYER_HMI_ITEM_H
