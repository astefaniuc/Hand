#ifndef HAND_GRAPH_METHOD_H
#define HAND_GRAPH_METHOD_H

#include "graph/hmiitem.h"
#include "view/layers/button.h"


template <class CallbackOwner>
class Action : public HmiItem
{
    typedef void (CallbackOwner::*TCallback)(HmiItem*);

public:
    Action(
        const std::string& name, const std::string& description,
        CallbackOwner* obj, TCallback func)
        : HmiItem(name, description)
    {
        m_Method = new CCallback<CallbackOwner>(obj, func);
        AddActivationClient(m_Method);
    }
    ~Action() { delete m_Method; }

private:
    Layer* CreateLayer() override { return new Layers::Button(); }

    CCallback<CallbackOwner>* m_Method;
};

#endif // HAND_GRAPH_METHOD_H
