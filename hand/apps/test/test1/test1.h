#ifndef HAND_APPS_TEST_TEST1_H
#define HAND_APPS_TEST_TEST1_H

#include "include/hand.h"


class App : public Module
{
public:
    App();

private:
    HmiItem* PrintSelf(HmiItem* method);
    HmiItem* ChangeButtonName(HmiItem* method);

    Collection m_Hmi;
};

#endif // HAND_APPS_TEST_TEST1_H
