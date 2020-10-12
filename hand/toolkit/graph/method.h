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
        const std::string& a_name, const std::string& a_description,
        CallbackOwner* a_obj, TCallback a_func)
        : HmiItem(a_name, a_description)
    {
        m_Method = new CCallback<CallbackOwner>(a_obj, a_func);
        AddActivationClient(m_Method);
    }
    ~Action() { delete m_Method; }

    Type GetType() const override { return HmiItem::EAction; }

private:
    Layer* CreateLayer() override { return new Layers::Button(); }

    CCallback<CallbackOwner>* m_Method;
};

#endif // HAND_GRAPH_METHOD_H
