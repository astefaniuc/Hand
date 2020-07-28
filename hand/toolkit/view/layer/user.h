#ifndef HAND_VIEW_LAYER_USER_H
#define HAND_VIEW_LAYER_USER_H

#include "view/layer/layer.h"
#include "graph/collection.h"
#include "graph/interface.h"


class EventHandler;
class Hand;
class ModuleLib;

/// Manages the items to be shown on screen and the input modes.
class CUser : public Module
{
public:
    CUser(EventHandler* input);
    virtual ~CUser();

    bool LoadApp(Note* path);
    HmiItem* GetHmi() { return &m_View; }
    bool GetCommand(HmiItem* func, int level);

private:
    EventHandler* m_Input;
    ModuleLib* m_ThemeLoader;
    Theme* m_Theme = nullptr;
    // May be one or two hands, using a vector is overdosed (TODO)
    std::vector<Hand*> m_Hands;
    std::vector<ModuleLib*> m_RunningApps;
    Interface m_View;
    Collection m_ViewStack;
    Collection m_Menu;
};

#endif // HAND_VIEW_LAYER_USER_H
