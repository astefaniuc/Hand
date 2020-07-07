#include "user.h"

#include "input/device.h"
#include "input/hand.h"
#include "view/screen.h"
#include "input/inputstate.h"
#include "graph/method.h"
#include "view/layer/layermanager.h"
#include "base/handserver.h"


CUser::CUser(EventHandler* a_input, Screen* a_output)
    : m_Input(a_input), m_UI("User", "System interface")
{
//    m_UI.get(THEME)->add(new Action<CUser>(LOADER, this, &CUser::LoadTheme));

    // TODO: load settings

    a_output->Add(this);

    Hand* right = m_Input->CreateHand(Device::Keyboard);
    if (!right->Init())
        // Show init screen
        dynamic_cast<MasterLayer*>(right->GetHmi()->GetLayer())->Show(right->GetHmi());
    // Add the exit function to the tree of available funcs
    // Request command at highest level
//    GetCommand(m_Exit, _Device->GetNumberOfKeys());
}


CUser::~CUser()
{
    for (Hand* h : m_Hands)
        delete h;
    // Don't delete screen
    Buffer = nullptr;
}


bool CUser::GetCommand(HmiItem* f, int level)
{
    if (f)
        // TODO: two hands
        return m_Hands[0]->GetInputState()->GetCommand(f, level);
    return false;
}

