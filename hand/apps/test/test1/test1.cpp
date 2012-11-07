#include "test1.h"


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
    add(Output);
    // Add controls
    folder = new List("Level 1");
    add(folder);

    // Set the dummy callback methods
    List* func_folder = new List("button 1");
    func_folder->add(new Method<App>("button 1", this, &App::PrintSelf));
    func_folder->add(new Note(DESCRIPTION, "l1b1: prints this to Output"));
    folder->add(func_folder);

    func_folder = new List("button 2");
    func_folder->add(new Method<App>("button 2", this, &App::PrintSelf));
    func_folder->add(new Note(DESCRIPTION, "l1b2: prints this to Output,\n with newline"));
    folder->add(func_folder);

    // Create sub-menu folder
    List* sub_menu = new List("Level 2");
    sub_menu->add(new Note(DESCRIPTION, "l1b3: calls sub-menu"));
    folder->add(sub_menu);
    // Set the dummy callback methods
    func_folder = new List("button 1");
    func_folder->add(new Method<App>("button 1", this, &App::PrintSelf));
    func_folder->add(new Note(DESCRIPTION, "l2b1: Func C bug"));
    sub_menu->add(func_folder);

    func_folder = new List("button 2");
    func_folder->add(new Method<App>("button 2", this, &App::PrintSelf));
    func_folder->add(new Note(DESCRIPTION, "l2b2: prints this to Output"));
    sub_menu->add(func_folder);

    func_folder = new List("button 3");
    func_folder->add(new Method<App>("button 3", this, &App::PrintSelf));
    func_folder->add(new Note(DESCRIPTION, "l2b3: prints this to Output"));
    sub_menu->add(func_folder);

    func_folder = new List("button 4");
    func_folder->add(new Method<App>("button 4", this, &App::PrintSelf));
    func_folder->add(new Note(DESCRIPTION, "l2b4: prints this to Output"));
    sub_menu->add(func_folder);

    func_folder = new List("button 5");
    func_folder->add(new Method<App>("button 5", this, &App::PrintSelf));
    func_folder->add(new Note(DESCRIPTION, "l2b5: prints this to Output"));
    sub_menu->add(func_folder);
}


bool App::PrintSelf(Vertex* self)
{
    Output->set(((Note*)(self->get(DESCRIPTION)))->get());
    return true;
}
