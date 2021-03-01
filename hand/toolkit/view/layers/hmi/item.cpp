#include "view/layers/hmi/item.h"
#include "input/interaction.h"


namespace Layers {


void Item::Rebuild()
{
    m_Name.SetData(m_Data->GetName());
    m_Info.SetData(m_Data->GetInfo());

    Insert(TITLE, &m_Name);
    if (!m_Data->GetInfo().empty())
        Insert(DESCRIPTION, &m_Info);
    if (GetCommand())
        Insert(CONTROL, GetCommand()->GetLayer());
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
