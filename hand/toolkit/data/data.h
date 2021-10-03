#ifndef HAND_GRAPH_DATA_H
#define HAND_GRAPH_DATA_H

#include "base/module.h"
#include "data/item.h"
#include "data/manipulator/manipulator.h"
#include <sstream>


namespace Data
{

class Base;


class Persistence
{
public:
    virtual ~Persistence() {}

    virtual bool SetFile(const std::string& path) = 0;
    virtual const std::string& GetFile() const = 0;

    virtual bool Load(Base* in) const = 0;
    virtual bool Save(Base* out) const = 0;
};


class Base : public Item
{
public:
    Base(const std::string& name, const std::string& description, Manipulator::Base* manip)
        : Item(name, description), m_Manipulator(manip) {}
    ~Base();

    virtual std::string GetValueString() = 0;
    virtual std::string GetTypeName() { return m_TypeName; };
    void SetTypeName(const std::string& name) { m_TypeName = name; }

    /// Deletes a previously stored manipulator.
    void SetManipulator(Manipulator::Base* manip);
    Manipulator::Base* GetManipulator();

    /// Deletes a previously stored persistence object.
    void SetPersistence(Persistence* storage);
    Persistence* GetPersistence() { return m_Storage; }

protected:
    void Execute(Layers::Item* caller) override;

    Layer* CreateExpandedData() override { return CreateCompressedData(); }
    Layer* CreateCompressedData() override;

    Persistence* m_Storage = nullptr;

    Manipulator::Base* m_Manipulator = nullptr;
    std::string m_TypeName;
};


template<typename DataType>
class Typed : public Base
{
public:
    Typed(
        const std::string& name,
        const std::string& description,
        const DataType& defaultValue,
        Manipulator::Typed<DataType>* manipulator = nullptr)
        : Base(name, description, manipulator), m_Value(defaultValue), m_DefaultValue(defaultValue)
    {
        if (manipulator)
            manipulator->SetItem(this);
    }

    virtual bool SetValue(const DataType& val)
    {
        if (m_Value == val)
            return true;
        // The manipulator should call this with valid data.
        // TODO: remove this? What about Persistence?
        if (m_Manipulator && !((Manipulator::Typed<DataType>*)m_Manipulator)->IsValid(val))
            return false;

        m_Value = val;
        NotifyChanged();
        return true;
    }
    bool operator=(const DataType& val) { return SetValue(val); }

    const DataType& GetValue() const { return m_Value; }
    const DataType& operator()() const { return m_Value; }

    const DataType& GetDefaultValue() const { return m_DefaultValue; }

    /// Get the value as a human readable string.
    std::string GetValueString() override
    {
        std::ostringstream s;
        s.flags(std::ios_base::dec | std::ios_base::boolalpha);
        s << m_Value;
        return s.str();
    }
    std::string GetTypeName() override { return ::GetTypeName<DataType>(); }

private:
    DataType m_Value;
    DataType m_DefaultValue;
};


template <typename DataType>
class Numeric : public Typed<DataType>
{
public:
    // Expose the Typed<DataType> ctor with default values for min & max.
    using Typed<DataType>::Typed;

    Numeric(
        const std::string& name,
        const std::string& description,
        const DataType& defaultValue,
        DataType min,
        DataType max,
        Manipulator::Typed<DataType>* manipulator = nullptr)
        : Typed<DataType>(name, description, manipulator)
    {
        static_assert(min <= defaultValue, "Value should be >= min");
        static_assert(defaultValue <= max, "Value should be <= max");
    }

    DataType GetMin() { return m_Min; }
    DataType GetMax() { return m_Max; }

private:
    DataType m_Min = std::numeric_limits<DataType>::min();
    DataType m_Max = std::numeric_limits<DataType>::max();
};


typedef Typed<bool> Bool;
typedef Typed<double> Double;
typedef Typed<float> Float;
typedef Typed<int> Int;
typedef Typed<int8_t> Int8;
typedef Typed<int16_t> Int16;
typedef Typed<int32_t> Int32;
typedef Typed<int64_t> Int64;
typedef Typed<std::string> String;
typedef Typed<uint8_t> Uint8;
typedef Typed<uint16_t> Uint16;
typedef Typed<uint32_t> Uint32;
typedef Typed<uint64_t> Uint64;
typedef Typed<unsigned> Unsigned;

}
#endif // HAND_GRAPH_DATA_H
