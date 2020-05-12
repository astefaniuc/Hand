#include "test1.h"


extern "C" Module* Create()
{
    // Export interface
    return (new App());
}


extern "C" void Destroy(Module* app)
{
    delete app;
}


App::App() : m_Hmi("Test1", "Simple test application")
{
    // The one and only view
    m_Hmi.Add(new HmiItem("Output field name", "<< Empty >>"));
    // Add controls
    Vertex* folder = get("Level 1");

    // Set the dummy callback methods
    m_Hmi.Add(new Action<App>("button 1", "l1b1: prints this to Output", this, &App::PrintSelf));
    m_Hmi.Add(new Action<App>("button 2", "l1b2: prints this to Output,\n with newline", this, &App::PrintSelf));

    // Create sub-menu folder
    Collection* sub = new Collection("Level 2", "l1b3: calls sub-menu");
    // Set the dummy callback methods
    sub->Add(new Action<App>("button 1", "l2b1: Func C bug", this, &App::PrintSelf));
    sub->Add(new Action<App>("button 2", "l2b2: prints this to Output", this, &App::PrintSelf));
    sub->Add(new Action<App>("button 3", "l2b3: Changes own button name", this, &App::ChangeButtonName));
    sub->Add(new Action<App>("button 4", "l2b4: prints this to Output", this, &App::PrintSelf));
    sub->Add(new Action<App>("button 5", "l2b5: prints this to Output", this, &App::PrintSelf));
}


HmiItem* App::PrintSelf(HmiItem* method)
{
    Output->set(((Note*)(method->get(DESCRIPTION)))->get());
    return m_Hmi;
}

HmiItem* App::ChangeButtonName(HmiItem* method)
{
    method->name("Executed");
    return m_Hmi;
}
