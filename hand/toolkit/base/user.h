#ifndef HAND_BASE_USER_H
#define HAND_BASE_USER_H

#include "data/vector.h"
#include "data/interface.h"


class EventHandler;
class Hand;
class ModuleLib;

/// Manages the items to be shown on screen and the input modes.
class User
{
public:
    User(EventHandler* input);
    virtual ~User();

    bool LoadApp(Hmi::Note* path);
    void Update();

    void Start();
    void Stop(Hmi::Item*);

private:
    EventHandler* m_Input;
    ModuleLib* m_ThemeLoader;
    // May be one or two hands, using a vector is overdosed (TODO)
    std::vector<Hand*> m_Hands;
    std::vector<ModuleLib*> m_RunningApps;
    Hmi::Interface m_View;
    Hmi::Vector m_ViewStack;
};

#endif // HAND_BASE_USER_H
