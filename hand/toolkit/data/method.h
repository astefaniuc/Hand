#ifndef HAND_GRAPH_METHOD_H
#define HAND_GRAPH_METHOD_H

#include "data/item.h"
#include "view/layers/button.h"


namespace Hmi {


template <class CallbackOwner>
class Action : public Item
{
    typedef void (CallbackOwner::*TCallback)(Item*);

public:
    Action(
        const std::string& name, const std::string& description,
        CallbackOwner* obj, TCallback func)
        : Item(name, description)
    {
        m_Method = new CCallback<CallbackOwner>(obj, func);
        AddActivationClient(m_Method);
    }
    ~Action() { delete m_Method; }

private:
    Layer* CreateExpandedView() override { return new Layers::Button(); }

    CCallback<CallbackOwner>* m_Method;
};

}
#endif // HAND_GRAPH_METHOD_H
