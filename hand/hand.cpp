#include "base/handserver.h"


int main(int argc, const char *argv[])
{
    HandServer* server = HandServer::GetInstance();

    if(argc == 2)
        server->Present(argv[1]);
    else
        server->GetUser();

    // Start the timer driven (callback) execution and stop
    // the current thread if not already done
    server->Start();

    // Never reached
    return 0;
}
