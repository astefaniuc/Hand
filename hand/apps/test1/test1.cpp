#include "test1.h"

#include <string>


extern "C" Functoid* Create()
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
    Functoid* folder;

    // The one and only view
    Output = new Note("Output field name", "<< Empty >>");
    Add(Output);
    // Add controls
    folder = new FunctoidNode("Level 1");
    Add(folder);

    // Set the dummy callback methods
    FunctoidNode* func_folder = new FunctoidNode("button 1");
    func_folder->Add(new Callback<App>("button 1", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l1b1: prints this to Output"));
    folder->Add(func_folder);

    func_folder = new FunctoidNode("button 2");
    func_folder->Add(new Callback<App>("button 2", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l1b2: prints this to Output,\n with newline"));
    folder->Add(func_folder);

    // Create sub-menu folder
    FunctoidNode* sub_menu = new FunctoidNode("Level 2");
    sub_menu->Add(new Note(TAG_DESCRIPTION, "l1b3: calls sub-menu"));
    folder->Add(sub_menu);
    // Set the dummy callback methods
    func_folder = new FunctoidNode("button 1");
    func_folder->Add(new Callback<App>("button 1", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l2b1: Func C bug"));
    sub_menu->Add(func_folder);

    func_folder = new FunctoidNode("button 2");
    func_folder->Add(new Callback<App>("button 2", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l2b2: prints this to Output"));
    sub_menu->Add(func_folder);

    func_folder = new FunctoidNode("button 3");
    func_folder->Add(new Callback<App>("button 3", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l2b3: prints this to Output"));
    sub_menu->Add(func_folder);

    func_folder = new FunctoidNode("button 4");
    func_folder->Add(new Callback<App>("button 4", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l2b4: prints this to Output"));
    sub_menu->Add(func_folder);

    func_folder = new FunctoidNode("button 5");
    func_folder->Add(new Callback<App>("button 5", this, &App::PrintSelf));
    func_folder->Add(new Note(TAG_DESCRIPTION, "l2b5: prints this to Output"));
    sub_menu->Add(func_folder);
}


bool App::PrintSelf(Functoid* self)
{
    Output->Set(((Note*)(self->Get(TAG_DESCRIPTION)))->Get());
    return true;
}
