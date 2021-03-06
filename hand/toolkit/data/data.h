#ifndef HAND_GRAPH_DATA_H
#define HAND_GRAPH_DATA_H

#include "base/module.h"
#include "data/item.h"
#include "data/manipulator/manipulator.h"
#include <sstream>


namespace Hmi {


class Data;


class Persistence
{
public:
    virtual ~Persistence() {}

    virtual bool SetFile(const std::string& path) = 0;
    virtual const std::string& GetFile() const = 0;

    virtual bool Load(Data* in) const = 0;
    virtual bool Save(Data* out) const = 0;
};


class Data : public Item
{
public:
    Data(const std::string& name, const std::string& description, Module* manipulator);
    ~Data();

    virtual std::string GetValueString() = 0;
    /// Deletes a previously stored manipulator.
    void SetManipulator(Module* manipulator);
    Module* GetManipulator() { return m_Manipulator; }

    /// Deletes a previously stored persistence object.
    void SetPersistence(Persistence* storage);
    Persistence* GetPersistence() { return m_Storage; }

protected:
    void Execute(Layers::Item* caller) override;

    Layer* CreateExpandedData() override { return CreateCompressedData(); }
    Layer* CreateCompressedData() override;

    Persistence* m_Storage = nullptr;

    Module* m_Manipulator = nullptr;
};


template<typename DataType>
class TData : public Data
{
public:
    TData(
        const std::string& name,
        const std::string& description,
        const DataType& defaultVal,
        Manipulator<DataType>* manipulator = nullptr)
        : Data(name, description, manipulator), m_Value(defaultVal)
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
        if (m_Manipulator && !((Manipulator<DataType>*)m_Manipulator)->IsValid(val))
            return false;

        m_Value = val;
        NotifyChanged();
        return true;
    }
    bool operator=(const DataType& val) { return SetValue(val); }

    const DataType& GetValue() const { return m_Value; }
    const DataType& operator()() const { return m_Value; }

    /// Get the value as a human readable string.
    std::string GetValueString() override
    {
        std::ostringstream s;
        s.flags(std::ios_base::dec | std::ios_base::boolalpha);
        s << m_Value;
        return s.str();
    }

private:
    DataType m_Value;
};


typedef TData<std::string> Note;

}
#endif // HAND_GRAPH_DATA_H
