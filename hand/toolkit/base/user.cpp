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
      m_ViewStack(new Layers::List()),
      m_Menu("Menu", "System")
{
    Layouts::Aligned::Map* layout = Layouts::Aligned::CreateView();
    layout->GetField(TITLE)->SetVisible(false);
    layout->GetField(DESCRIPTION)->SetVisible(false);
    m_View.GetLayer()->SetLayout(layout);

    m_View.SetView(m_ViewStack);
    m_View.SetControls(&m_Menu);

    m_ThemeLoader = new ModuleLib();
    m_Menu.Add(new Note(
            "Theme", "Select visualization theme",
            "./binaries/lib/themes/basicsdl1.so", m_ThemeLoader));

    // TODO: load settings
    Theme* theme = static_cast<Theme*>(m_ThemeLoader->GetObject());
    m_View.GetLayer()->SetTheme(theme);
    theme->InitScreen(m_View.GetLayer());
    m_Menu.Attach(theme->GetHmi()->GetContent());

    Hand* right =  new Hand(m_Input->GetDevice(Device::Keyboard));
    if (!right->Init())
        // Show init screen
        m_ViewStack->Insert(right->GetHmi());
    // Add the exit function to the tree of available funcs
    // Request command at highest level
//    GetCommand(m_Exit, _Device->GetNumberOfKeys());

    m_Input->SetUser(this);
    m_Input->Start();
}


User::~User()
{
    for (ModuleLib* app : m_RunningApps)
        delete app;
    for (Hand* h : m_Hands)
        delete h;
    delete m_ViewStack;
}


bool User::LoadApp(Note* a_path)
{
    ModuleLib* app = new ModuleLib();
    app->SetItem(a_path);
    if (app->Load())
    {
        m_RunningApps.push_back(app);
        m_ViewStack->Insert(app->GetHmi());
        return true;
    }

    delete app;
    return false;
}


void User::Update()
{
    m_View.GetLayer()->Update();
    m_View.GetLayer()->GetTheme()->UpdateScreen();
}


bool User::GetCommand(HmiItem* f, int level)
{
    if (f)
        // TODO: two hands
        return m_Hands[0]->GetInputState()->GetCommand(f, level);
    return false;
}

