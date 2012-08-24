#include "test1.h"

#include <string>


extern "C" Vertex* Create()
{
    // Export interface
    return (new App());
}


extern "C" void Destroy(HandApp* app)
{
    delete app;
}


void App::Init()
{
    Vertex* folder;

    // The one and only view
    Output = new Note("Output field name", "<< Empty >>");
    Add(Output);
    // Add controls
    folder = new List("Level 1");
    Add(folder);

    // Set the dummy callback methods
    List* func_folder = new List("button 1");
    func_folder->Add(new Method<App>("button 1", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l1b1: prints this to Output"));
    folder->Add(func_folder);

    func_folder = new List("button 2");
    func_folder->Add(new Method<App>("button 2", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l1b2: prints this to Output,\n with newline"));
    folder->Add(func_folder);

    // Create sub-menu folder
    List* sub_menu = new List("Level 2");
    sub_menu->Add(new Note(TAG_DESCRIPTION, "l1b3: calls sub-menu"));
    folder->Add(sub_menu);
    // Set the dummy callback methods
    func_folder = new List("button 1");
    func_folder->Add(new Method<App>("button 1", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l2b1: Func C bug"));
    sub_menu->Add(func_folder);

    func_folder = new List("button 2");
    func_folder->Add(new Method<App>("button 2", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l2b2: prints this to Output"));
    sub_menu->Add(func_folder);

    func_folder = new List("button 3");
    func_folder->Add(new Method<App>("button 3", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l2b3: prints this to Output"));
    sub_menu->Add(func_folder);

    func_folder = new List("button 4");
    func_folder->Add(new Method<App>("button 4", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l2b4: prints this to Output"));
    sub_menu->Add(func_folder);

    func_folder = new List("button 5");
    func_folder->Add(new Method<App>("button 5", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l2b5: prints this to Output"));
    sub_menu->Add(func_folder);
}


bool App::PrintSelf(Vertex* self)
{
    Output->Set(((Note*)(self->Get(TAG_DESCRIPTION)))->Get());
    return true;
}
