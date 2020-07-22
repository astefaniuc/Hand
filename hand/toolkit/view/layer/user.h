#ifndef HAND_VIEW_LAYER_USER_H
#define HAND_VIEW_LAYER_USER_H

#include "view/layer/layer.h"
#include "graph/collection.h"


class EventHandler;
class Hand;
class ModuleLib;
class Screen;

/// Manages the items to be shown on screen and the input modes.
class CUser : public Layer
{
public:
    CUser(EventHandler* input, Screen* output);
    virtual ~CUser();

    bool LoadApp(Note* path);
    bool GetCommand(HmiItem* func, int level);

private:
    EventHandler* m_Input;
    ModuleLib* m_ThemeLoader;
    Theme* m_Theme = nullptr;
    // May be one or two hands, using a vector is overdosed (TODO)
    std::vector<Hand*> m_Hands;
    std::vector<ModuleLib*> m_RunningApps;
    Collection m_UI;
};

#endif // HAND_VIEW_LAYER_USER_H
