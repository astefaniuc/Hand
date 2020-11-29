#include "user.h"
#include "input/eventhandler.h"
#include "view/layers/list.h"
#include "input/device.h"
#include "input/hand.h"
#include "view/theme.h"
#include "input/inputstate.h"
#include "data/method.h"
#include "data/data.h"
#include "base/modulelib.h"
#include "view/layouts/aligned.h"


User::User(EventHandler* a_input)
    : m_Input(a_input),
      m_View("User", "User view"),
      m_ViewStack("View", "")
{
    Layouts::Aligned::Map* layout = Layouts::Aligned::CreateView();
    layout->GetField(TITLE)->SetVisible(false);
    layout->GetField(DESCRIPTION)->SetVisible(false);
    m_View.GetExpandedView()->SetLayout(layout);

    m_View.SetView(m_ViewStack.GetExpandedView());

    static_cast<Layers::List*>(m_ViewStack.GetExpandedView())->SetExpandChildren(true);

    m_ThemeLoader = new ModuleLib();
    m_View.GetControls().Add(new Hmi::Note(
            "Theme", "Select visualization theme",
            "./binaries/lib/themes/basicsdl1.so", m_ThemeLoader));

    // TODO: load settings
    Theme* theme = static_cast<Theme*>(m_ThemeLoader->GetObject());
    m_View.GetExpandedView()->SetTheme(theme);
    theme->InitScreen(m_View.GetExpandedView());
    m_View.GetControls().Attach(theme->GetHmi());

    Hand* right = new Hand(m_Input->GetDevice(Device::Keyboard));
    if (!right->Init())
    {
        // Show init screen
        Hmi::Item* initView = right->GetInitScreen();
        m_ViewStack.Attach(initView);
        m_View.GetExpandedView()->Update();
        right->SetFocus(initView->GetExpandedView());
    }

    m_Input->SetUser(this);
    m_Input->Start();
}


User::~User()
{
    for (ModuleLib* app : m_RunningApps)
        delete app;
    for (Hand* h : m_Hands)
        delete h;
}


bool User::LoadApp(Hmi::Note* a_path)
{
    ModuleLib* app = new ModuleLib();
    app->SetItem(a_path);
    if (app->Load())
    {
        m_RunningApps.push_back(app);
        m_ViewStack.Attach(app->GetHmi());
        return true;
    }

    delete app;
    return false;
}


void User::Update()
{
    m_View.GetExpandedView()->Update();
    m_View.GetExpandedView()->GetTheme()->UpdateScreen();
}
