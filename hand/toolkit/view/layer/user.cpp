#include "user.h"

#include "input/device.h"
#include "input/hand.h"
#include "view/screen.h"
#include "input/inputstate.h"
#include "graph/method.h"
#include "view/layer/layermanager.h"
#include "base/handapploader.h"


CUser::CUser(EventHandler* a_input, Screen* a_output)
    : m_Input(a_input), m_UI("User", "System interface")
{
//    m_UI.get(THEME)->add(new Action<CUser>(LOADER, this, &CUser::LoadTheme));

    // TODO: load settings

    a_output->Add(this);

    Hand* right = m_Input->CreateHand(Device::Keyboard);
    if (!right->Init())
        // Show init screen
        SetContent(right->GetHmi());
    // Add the exit function to the tree of available funcs
    // Request command at highest level
//    GetCommand(m_Exit, _Device->GetNumberOfKeys());
}


CUser::~CUser()
{
    for (ModuleLib* app : m_RunningApps)
        delete app;
    for (Hand* h : m_Hands)
        delete h;
    // Don't delete screen
    Buffer = nullptr;
}


bool CUser::LoadApp(Note* a_path)
{
    ModuleLib* app = new ModuleLib();
    app->SetItem(a_path);
    if (app->Load())
    {
        m_RunningApps.push_back(app);
        SetContent(app->GetHmi());
        return true;
    }

    delete app;
    return false;
}

bool CUser::GetCommand(HmiItem* f, int level)
{
    if (f)
        // TODO: two hands
        return m_Hands[0]->GetInputState()->GetCommand(f, level);
    return false;
}

