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
    // The one and only view
    Output = new Note("Output field name", "<< Empty >>");

    // HACK bug fix: use an intermediate list with frames list layout for clearing
    // the content before drawing new text
    Vertex* frame = get("Output");
    frame->Vertex::get(LAYOUT)->get(LIST)->set(
            Vertex::get(FACTORY, THEMES)->get(DEFAULT)->get(LAYOUT)->get(LIST)->get(FRAMEDLIST)->get());
    frame->add(Output);
    // Add controls
    Vertex* folder = get("Level 1");

    // Set the dummy callback methods
    Vertex* button = new Method<App>("button 1", this, &App::PrintSelf);
    button->add(new Note(DESCRIPTION, "l1b1: prints this to Output"));
    folder->add(button);

    button = new Method<App>("button 2", this, &App::PrintSelf);
    button->add(new Note(DESCRIPTION, "l1b2: prints this to Output,\n with newline"));
    folder->add(button);

    // Create sub-menu folder
    folder = folder->get("Level 2");
    folder->Vertex::add(new Note(DESCRIPTION, "l1b3: calls sub-menu"));
    // Set the dummy callback methods
    button = new Method<App>("button 1", this, &App::PrintSelf);
    button->add(new Note(DESCRIPTION, "l2b1: Func C bug"));
    folder->add(button);

    button = new Method<App>("button 2", this, &App::PrintSelf);
    button->add(new Note(DESCRIPTION, "l2b2: prints this to Output"));
    folder->add(button);

    button = new Method<App>("button 3", this, &App::ChangeButtonName);
    button->add(new Note(DESCRIPTION, "l2b3: Changes own button name"));
    folder->add(button);

    button = new Method<App>("button 4", this, &App::PrintSelf);
    button->add(new Note(DESCRIPTION, "l2b4: prints this to Output"));
    folder->add(button);

    button = new Method<App>("button 5", this, &App::PrintSelf);
    button->add(new Note(DESCRIPTION, "l2b5: prints this to Output"));
    folder->add(button);
}


bool App::PrintSelf(Vertex* method)
{
    Output->set(((Note*)(method->get(DESCRIPTION)))->get());
    return true;
}

bool App::ChangeButtonName(Vertex* method)
{
    method->name("Executed");
    return true;
}
