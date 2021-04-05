#ifndef HAND_DATA_MANIPULATOR_MANIPULATOR_H
#define HAND_DATA_MANIPULATOR_MANIPULATOR_H

#include "base/module.h"
#include <limits>


namespace Hmi {
    class Data;
    template<typename DataType> class TData;
    class Interface;
}


namespace Manipulator {


class Base : public Module
{
public:
    void SetInterface(Hmi::Interface* hmi) { m_Interface = hmi; }
    void GetHmi(Layer* caller) override;

protected:
    Hmi::Interface* m_Interface = nullptr;
};


template <typename DataType>
class Typed : public Base
{
public:
    void SetItem(Hmi::TData<DataType>* toHandle)
    {
        m_Item = toHandle;
        Init();
    }
    virtual bool IsValid(const DataType& /*input*/) { return true; }

    virtual void Init() {}

protected:
    Hmi::TData<DataType>* m_Item = nullptr;
};


template <typename DataType>
class Numeric : public Typed<DataType>
{
public:
    void SetMin(DataType min);
    void SetMax(DataType max);
    bool IsValid(const DataType& input) override { return ((input >= m_Min) && (input <= m_Max)); }

protected:
    DataType m_Min = std::numeric_limits<DataType>::min();
    DataType m_Max = std::numeric_limits<DataType>::max();
};

}
#endif // HAND_DATA_MANIPULATOR_MANIPULATOR_H
