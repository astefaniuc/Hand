#ifndef HAND_VIEW_LAYER_USER_H
#define HAND_VIEW_LAYER_USER_H

#include "graph/collection.h"
#include "graph/interface.h"


class EventHandler;
class Hand;
class ListLayer;
class ModuleLib;

/// Manages the items to be shown on screen and the input modes.
class User
{
public:
    User(EventHandler* input);
    virtual ~User();

    bool LoadApp(Note* path);
    bool GetCommand(HmiItem* func, int level);
    void Update();

private:
    EventHandler* m_Input;
    ModuleLib* m_ThemeLoader;
    // May be one or two hands, using a vector is overdosed (TODO)
    std::vector<Hand*> m_Hands;
    std::vector<ModuleLib*> m_RunningApps;
    Interface m_View;
    ListLayer* m_ViewStack;
    Collection m_Menu;
};

#endif // HAND_VIEW_LAYER_USER_H
