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
    m_ViewLayer->ExitListeners.Add(this, &User::StopCb);
    m_ViewLayer->SetData(&m_View);

    m_View.AddFactory(new TypeFactory<Manipulator::Typed<uint8_t>, Manipulator::Base>(), GetTypeName<uint8_t>());

    m_ThemeLoader = new ModuleLib();
    m_View.AddControl(new Data::String(
            "Theme", "Select visualization theme",
            "./binaries/lib/themes/basicsdl1.so", m_ThemeLoader));

    // TODO: load settings
    Theme* theme = static_cast<Theme*>(m_ThemeLoader->GetObject());
    m_ViewLayer->SetTheme(theme);
    theme->InitScreen(m_ViewLayer);
    theme->GetHmi(m_ViewLayer);


    m_Control = new Interaction::Control(new Hand(m_Input->GetDevice(Device::Keyboard)));
    m_Control->SetTarget(m_ViewLayer);

    m_Input->SetUser(this);
}


User::~User()
{
    delete m_Control;
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
}


void User::Stop()
{
    m_ViewLayer->Exit();
}


bool User::LoadApp(Data::String* a_path)
{
    ModuleLib* app = new ModuleLib();
    app->SetItem(a_path);
    if (app->Load())
    {
        m_RunningApps.push_back(app);
        app->GetHmi(m_ViewLayer);
        return true;
    }

    delete app;
    return false;
}


void User::Update(bool forced)
{
    m_ViewLayer->Prune();
    if (!m_ViewLayer)
    {
        m_Input->Stop();
        m_MainThread.notify_all();
        return;
    }
    m_Control->Update();
    m_ViewLayer->GetTheme()->UpdateScreen(forced);
}
