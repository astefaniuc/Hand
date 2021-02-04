#ifndef HAND_GRAPH_METHOD_H
#define HAND_GRAPH_METHOD_H

#include "data/item.h"
#include "view/layers/button.h"


namespace Hmi {


template <class CbOwner>
class Action : public Item
{
    typedef void (CbOwner::*TCallback)(Item*);

public:
    Action(
        const std::string& name, const std::string& description,
        CbOwner* obj, TCallback func)
        : Item(name, description)
    {
        ActivationListeners.Add(obj, func);
    }

//Parameter...
private:
    Layer* CreateExpandedView() override { return new Layers::Button(); }
};

}
#endif // HAND_GRAPH_METHOD_H
