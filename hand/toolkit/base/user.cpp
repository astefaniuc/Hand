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
#include "view/layers/view.h"
#include "view/layouts/builtin.h"


User::User(EventHandler* a_input)
    : m_Input(a_input),
      m_View("User", "User view"),
      m_ViewStack("View", "")
{
    Layers::List* baseView = static_cast<Layers::List*>(m_View.GetExpandedView());

    Layout* layout = Layouts::CreateView();
    layout->GetField(VIEW)->SetExpanding(true, true);
    layout->GetField(TITLE)->SetVisible(false);
    layout->GetField(DESCRIPTION)->SetVisible(false);
    baseView->SetLayout(layout);

    m_View.SetView(&m_ViewStack);

    static_cast<Layers::Vector*>(m_ViewStack.GetExpandedView())->SetExpandChildren(true);

    m_ThemeLoader = new ModuleLib();
    m_View.AddControl(new Hmi::Note(
            "Theme", "Select visualization theme",
            "./binaries/lib/themes/basicsdl1.so", m_ThemeLoader));

    // TODO: load settings
    Theme* theme = static_cast<Theme*>(m_ThemeLoader->GetObject());
    baseView->SetTheme(theme);
    theme->InitScreen(baseView);
    m_View.AttachControl(theme->GetHmi());

    m_Hands.push_back(new Hand(m_Input->GetDevice(Device::Keyboard)));
    m_Hands[0]->SetFocus(baseView);

    m_Input->SetUser(this);
}


User::~User()
{
    for (ModuleLib* app : m_RunningApps)
        delete app;
    for (Hand* h : m_Hands)
        delete h;
}


void User::Start()
{
    static_cast<Layers::View*>(m_View.GetExpandedView())->
        AddOnExit(new CCallback<User>(this, &User::Stop));

    if (!m_Hands[0]->Init())
    {
        // Show init screen
        Hmi::Item* initView = m_Hands[0]->GetInitScreen();
        m_ViewStack.Attach(initView);
        m_Hands[0]->SetFocus(initView->GetExpandedView());
    }

    m_Input->Start();
    // Stop the main execution line
    std::unique_lock<std::mutex> lock(m_Mutex);
    m_MainThread.wait(lock);

    m_Input->Stop();
    m_View.GetExpandedView()->Exit();
}


bool User::LoadApp(Hmi::Note* a_path)
{
    ModuleLib* app = new ModuleLib();
    app->SetItem(a_path);
    if (app->Load())
    {
        m_RunningApps.push_back(app);
        Hmi::Item* hmi = app->GetHmi();
        m_ViewStack.Attach(hmi);
        m_Hands[0]->SetFocus(hmi->GetExpandedView());
        return true;
    }

    delete app;
    return false;
}


void User::Update()
{
    m_View.GetExpandedView()->GetTheme()->UpdateScreen();
}
