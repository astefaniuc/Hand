#ifndef HAND_VIEW_LAYER_USER_H
#define HAND_VIEW_LAYER_USER_H

#include "graph/interface.h"
#include "view/layer/layer.h"


class EventHandler;
class Screen;
class Hand;

/// Manages the items to be shown on screen and the input modes.
class CUser : public Layer
{
public:
    CUser(EventHandler* input, Screen* output);
    virtual ~CUser();

    bool GetCommand(HmiItem* func, int level);

private:
    EventHandler* m_Input;
    // May be one or two hands, using a vector is overdosed (TODO)
    std::vector<Hand*> m_Hands;
    Interface m_UI;
};

#endif // HAND_VIEW_LAYER_USER_H
