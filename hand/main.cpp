#include "base/handserver.h"


int main(int argc, const char *argv[])
{
    std::string app;
    if (argc == 2)
        app = argv[1];

    HandServer server(app);
    // Start the timer driven (callback) execution and stop
    // the current thread if not already done
    server.Start();

    // Never reached
    return 0;
}
