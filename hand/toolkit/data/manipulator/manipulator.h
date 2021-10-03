#ifndef HAND_DATA_MANIPULATOR_MANIPULATOR_H
#define HAND_DATA_MANIPULATOR_MANIPULATOR_H

#include "base/module.h"
#include "base/typenames.h"
#include <limits>


namespace Data {
    class Base;
    template<typename DataType> class Typed;
    class Interface;
}


namespace Manipulator {


class Base : public Module
{
public:
    void SetInterface(Data::Interface* hmi) { m_Interface = hmi; }
    void GetHmi(Layer* caller) override;

protected:
    Data::Interface* m_Interface = nullptr;
};


template <typename DataType>
class Typed : public Base
{
public:
    void SetItem(Data::Typed<DataType>* toHandle)
    {
        m_Item = toHandle;
        Init();
    }
    virtual bool IsValid(const DataType& /*input*/) { return true; }

    virtual void Init() {}

protected:
    Data::Typed<DataType>* m_Item = nullptr;
};


template <typename DataType>
class Numeric : public Typed<DataType>
{
public:
    bool IsValid(const DataType& input) override
    {
        return (input >= this->m_Item->GetMin()) && (input <= this->m_Item->GetMax());
    }
};

}
#endif // HAND_DATA_MANIPULATOR_MANIPULATOR_H
