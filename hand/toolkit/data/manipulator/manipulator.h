#ifndef HAND_DATA_MANIPULATOR_MANIPULATOR_H
#define HAND_DATA_MANIPULATOR_MANIPULATOR_H

#include "base/module.h"


namespace Hmi {


class Data;
template<typename DataType> class TData;
class Interface;

// Helper to avoid circular include
void Show(Layer* target, Interface* view);

template <typename DataType>
class Manipulator : public Module
{
public:
    void SetItem(TData<DataType>* toHandle)
    {
        m_Item = toHandle;
        Init();
    }
    virtual bool IsValid(const DataType& input) = 0;

    virtual void Init() {}

    void GetHmi(Layer* caller) override
    {
        Show(caller, m_Interface);
    }

protected:
    TData<DataType>* m_Item = nullptr;
    Interface* m_Interface = nullptr;
};

}
#endif // HAND_DATA_MANIPULATOR_MANIPULATOR_H
