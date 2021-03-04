#include "view/layers/hmi/item.h"
#include "view/layers/text.h"
#include "input/interaction.h"


namespace Layers {


void Item::Rebuild()
{
    Insert(TITLE, new Text(m_Data->GetName()));
    if (!m_Data->GetInfo().empty())
        Insert(DESCRIPTION, new Text(m_Data->GetInfo()));
    if (GetCommand())
        Insert(CONTROL, GetCommand()->GetLayer());
}


void Item::SetData(Hmi::Item* data)
{
    Map::SetData(data);
    ActivationListeners.Add(data, &Hmi::Item::Execute);
}


bool Item::SetCommand(Interaction::Command* ctrl)
{
    m_Command = ctrl;
    Insert(CONTROL, GetCommand()->GetLayer());
    return true;
}


void Item::ReleaseCommand()
{
    if (!m_Command)
        return;
    GetCommand()->GetLayer()->SetParentField(nullptr);
    m_Command = nullptr;
}

}
