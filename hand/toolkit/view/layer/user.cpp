#include "user.h"

#include "input/device.h"
#include "input/inputstate.h"
#include "graph/method.h"
#include "view/layer/layermanager.h"


CUser::CUser() : m_UI("User", "System interface")
{
//    m_UI.get(THEME)->add(new Action<CUser>(LOADER, this, &CUser::LoadTheme));
}


CUser::~CUser()
{
    delete(_Device);
    // Don't delete screen
    Buffer = nullptr;
}


bool CUser::GetCommand(HmiItem* f, int level)
{
    if(f)
        return _Device->GetInputState()->GetCommand(f, level);
    return false;
}


void CUser::SetDevice(Device* device)
{
    _Device = device;
    if (!device->Init())
        // Show init screen
        dynamic_cast<MasterLayer*>(device->GetHmi()->GetLayer())->Show(device->GetHmi());
    // Add the exit function to the tree of available funcs
    // Request command at highest level
//    GetCommand(m_Exit, _Device->GetNumberOfKeys());
}
