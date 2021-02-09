#ifndef HAND_BASE_USER_H
#define HAND_BASE_USER_H

#include "data/vector.h"
#include "data/interface.h"
#include <mutex>
#include <condition_variable>


class EventHandler;
class ModuleLib;
namespace Interaction { class Control; }
namespace Layers { class Interface; }

/// Manages the items to be shown on screen and the input modes.
class User
{
public:
    User(EventHandler* input);
    virtual ~User();

    bool LoadApp(Hmi::Note* path);
    void Update();

    void Start();
    void Stop(Layers::Interface*){ m_MainThread.notify_all(); }

private:
    EventHandler* m_Input;
    ModuleLib* m_ThemeLoader;

    Interaction::Control* m_Control;

    std::vector<ModuleLib*> m_RunningApps;
    Hmi::Interface m_View;

    std::mutex m_Mutex;
    std::condition_variable m_MainThread;
};

#endif // HAND_BASE_USER_H
