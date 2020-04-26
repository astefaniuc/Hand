#ifndef HAND_APPS_TEST_TEST1_H
#define HAND_APPS_TEST_TEST1_H

#include "include/hand.h"


class App : public HandApp
{
public:
    App() : HandApp("Gui_Test") { Init(); }

private:
    bool PrintSelf(Vertex* method);
    bool ChangeButtonName(Vertex* method);
    void Init();

    Note* Output;
};

#endif // HAND_APPS_TEST_TEST1_H
