#ifndef HANDAPP_TEST1_H
#define HANDAPP_TEST1_H
#include "base/handapp.h"


using namespace std;


class App : public HandApp
{
    public:
        App(void) : HandApp("Gui_Test") {Init();};
    private:
        bool PrintSelf(Functoid* self);
        void Init(void);
        Note* Output;
};

#endif /* HANDAPP_TEST1_H */
