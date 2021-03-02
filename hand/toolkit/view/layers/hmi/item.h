#ifndef HAND_VIEW_LAYER_HMI_ITEM_H
#define HAND_VIEW_LAYER_HMI_ITEM_H

#include "view/layers/map.h"
#include "view/layers/text.h"


namespace Layers {


class Item : public Map
{
public:
    ~Item() { Exit(); }

    void SetData(Hmi::Item* data) override;

    bool SetCommand(Interaction::Command* ctrl) override;
    Interaction::Command* GetCommand() override { return m_Command; }
    void ReleaseCommand() override;

    void Activate() override { ActivationListeners.Execute(this); }

    Listeners<Layers::Item> ActivationListeners;

protected:
    void Rebuild() override;

    Text m_Name;
    Text m_Info;

    Interaction::Command* m_Command = nullptr;
};

}
#endif // HAND_VIEW_LAYER_HMI_ITEM_H
