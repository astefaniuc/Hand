#include "input/sdl/eventhandler.h"
#include "base/user.h"
#include <iostream>


int main(int argc, const char *argv[])
{
    std::string app;
    if (argc == 2)
        app = argv[1];

    User user(new EventHandlerSdl());
    // Start the timer driven (callback) execution and stop
    // the current thread if not already done
    if (!app.empty())
    {
        Hmi::Note* appPath = new Hmi::Note("Start app", "Passed as program argument", app);
        if (!user.LoadApp(appPath))
            std::cerr << "Error: can't open app '" << app << "'.";
    }

    // Never reached
    return 0;
}
