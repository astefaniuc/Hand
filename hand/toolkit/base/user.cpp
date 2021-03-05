#include "user.h"
#include "base/modulelib.h"
#include "data/method.h"
#include "data/data.h"
#include "input/device.h"
#include "input/eventhandler.h"
#include "input/hand.h"
#include "input/inputstate.h"
#include "view/theme.h"
#include "view/layers/vector.h"
#include "view/layers/hmi/interface.h"
#include "view/layouts/builtin.h"


User::User(EventHandler* a_input)
    : m_Input(a_input),
      m_View("User", "User view")
{
    Layout* layout = Layouts::CreateView();
    layout->GetField(VIEW)->SetExpanding(true, true);
    layout->GetField(TITLE)->SetVisible(false);
    layout->GetField(DESCRIPTION)->SetVisible(false);

    m_ViewLayer = new Layers::Interface();
    m_ViewLayer->SetLayout(layout);
    m_ViewLayer->ExitListeners.Add(this, &User::Stop);
    m_ViewLayer->SetData(&m_View);

    m_ThemeLoader = new ModuleLib();
    m_View.AddControl(new Hmi::Note(
            "Theme", "Select visualization theme",
            "./binaries/lib/themes/basicsdl1.so", m_ThemeLoader));

    // TODO: load settings
    Theme* theme = static_cast<Theme*>(m_ThemeLoader->GetObject());
    m_ViewLayer->SetTheme(theme);
    theme->InitScreen(m_ViewLayer);
    m_View.AttachControl(theme->GetHmi());


    m_Control = new Interaction::Control(new Hand(m_Input->GetDevice(Device::Keyboard)));
    m_Control->SetTarget(m_ViewLayer);

    m_Input->SetUser(this);
}


User::~User()
{
    for (ModuleLib* app : m_RunningApps)
        delete app;
}


void User::Start()
{
    m_Control->Start();

    m_Input->Start();
    // Stop the main execution line
    std::unique_lock<std::mutex> lock(m_Mutex);
    m_MainThread.wait(lock);

    m_Input->Stop();
}


void User::Stop(Layer*)
{
    m_ViewLayer = nullptr;
    m_MainThread.notify_all();
}


bool User::LoadApp(Hmi::Note* a_path)
{
    ModuleLib* app = new ModuleLib();
    app->SetItem(a_path);
    if (app->Load())
    {
        m_RunningApps.push_back(app);
        m_ViewLayer->Show(app->GetHmi()->GetExpandedView());
        return true;
    }

    delete app;
    return false;
}


void User::Update()
{
    if (m_ViewLayer)
        m_ViewLayer->GetTheme()->UpdateScreen();
}
