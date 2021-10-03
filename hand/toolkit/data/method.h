#ifndef HAND_DATA_METHOD_H
#define HAND_DATA_METHOD_H

#include "data/item.h"
#include "view/layers/hmi/button.h"


namespace Data {


template <class CbOwner>
class Action : public Item
{
    typedef void (CbOwner::*TCallback)(Layers::Item*);

public:
    Action(
        const std::string& name, const std::string& description,
        CbOwner* obj, TCallback func)
        : Item(name, description), m_Object(obj), m_Function(func) {}

private:
    Layer* CreateExpandedView() override { return CreateButtonView(); }

    void Execute(Layers::Item* caller) final
    {
        if (m_Object && m_Function)
            (m_Object->*m_Function)(caller);
    }

    CbOwner* m_Object;
    TCallback m_Function;
};

}
#endif // HAND_DATA_METHOD_H
