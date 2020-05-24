#include "user.h"

#include "input/device.h"
#include "input/inputstate.h"
#include "view/datatypes/layout.h"
#include "graph/method.h"


CUser::CUser() : m_UI(USER)
{
    m_UI.get(THEME)->add(new Method<CUser>(LOADER, this, &CUser::LoadTheme));
    m_UI.add(new Method<CUser>("Exit", this, &CUser::Exit));

    MainView = new Layer();
}


CUser::~CUser()
{
    delete(_Device);
    // Don't delete screen
    Buffer = nullptr;
}


void CUser::Init()
{
    // Load the theme
    LoadTheme(Vertex::get(FACTORY, THEMES)->get(DEFAULT));

    // Add the exit function to the tree of available funcs
    // Request command at highest level
    GetCommand(m_UI.get("Exit"), _Device->GetNumberOfKeys());
    MainView->SetContent(&m_UI);
}


bool CUser::Update(bool forced)
{
    if(NextRequest)
    {
        if(MainView)
        {
            delete(MainView);
            MainView = nullptr;
        }
        if(Expand(NextRequest))
            forced = true;
    }
    NextRequest = nullptr;
    return Layer::Update(forced);
}


bool CUser::Expand(HmiItem* to_expand)
{
    MainView = Insert(to_expand, VIEW);
    if(!MainView)
        return false;
    MainView->get(EXECUTE)->get()->execute(nullptr);
    return true;
}


bool CUser::GetCommand(Vertex* f, int level)
{
    if(f)
        return _Device->GetInputState()->GetCommand(f, level);
    return false;
}


bool CUser::Exit(Vertex* content)
{
    content = GetContent();
    // Check if the default location is currently active
    if(MainView && (MainView->GetContent()!=content))
        return Request(content);

    // Suicide
    delete(this);
    return false;
}


void CUser::SetDevice(Device* device)
{
    _Device = device;
    device->Vertex::get(LAYERMANAGER)->set(this);
    if(!device->Init())
        // No, show init screen
        Request(device->get(VIEW));
    else
        // TODO: load controls vertices also with the init screen
        Expand(device->Vertex::get(KEYLIST));
    // Delayed initialization of the CUser, needs device ptr
    // TODO: handle switching devices
    Init();
}


bool CUser::Request(Vertex* request)
{
    NextRequest = request;
    return true;
}
