#ifndef HAND_GRAPH_DATA_H
#define HAND_GRAPH_DATA_H

#include "base/module.h"
#include "graph/vertex.h"


class Data;


class Persistence
{
public:
    virtual ~Persistence() {}

    virtual bool SetFile(const std::string& a_path) = 0;
    virtual const std::string& GetFile() const = 0;

    virtual bool Load(Data* a_in) const = 0;
    virtual bool Save(Data* a_out) const = 0;
};


class Data : public HmiItem
{
public:
    Data(const std::string& a_name, const std::string& a_description, Module* a_manipulator)
        : HmiItem(a_name, a_description), m_Manipulator(a_manipulator)
    {
//        if (m_Manipulator)
//            AddActivationClient(m_Manipulator->GetHmi());
    }
    ~Data() { delete m_Manipulator; }

    /// The callback is executed whenever the selection changes.
    void AddDataChangedClient(ICallback* a_client)
    {
        m_DataChanged.push_back(a_client);
    }
    void RemoveDataChangedClient(ICallback* a_client)
    {
        RemoveCallback(a_client, m_DataChanged);
    }

    /// Deletes a previously stored manipulator.
    void SetManipulator(Module* a_manipulator);
    Module* GetManipulator() { return m_Manipulator; }

    /// Deletes a previously stored persistence object.
    void SetPersistence(Persistence* a_storage);
    Persistence* GetPersistence() { return m_Storage; }

protected:
    Persistence* m_Storage = nullptr;

    Listeners m_DataChanged;
    Module* m_Manipulator = nullptr;
};


template<typename DataType> class TData;


template <typename DataType>
class Manipulator : public Module
{
public:
    void SetItem(TData<DataType>* a_toHandle) { m_Item = a_toHandle; }
    // TEMP?
    virtual bool IsValid(const DataType& a_input) = 0;

    Interface* GetHmi() override { return m_Interface; }
    void SetHmi(Interface* interface) { m_Interface = interface; }

protected:
    TData<DataType>* m_Item = nullptr;
    Interface* m_Interface = nullptr;
};


template<typename DataType>
class TData : public Data
{
public:
    TData(
        const std::string& a_name,
        const std::string& a_description,
        const DataType& a_defaultVal,
        Manipulator<DataType>* a_manipulator = nullptr)
        : Data(a_name, a_description, a_manipulator), m_Value(a_defaultVal)
    {
        if (a_manipulator)
            a_manipulator->SetItem(this);
    }

    virtual bool SetValue(const DataType& a_val)
    {
        // The manipulator should call this with valid data.
        // TODO: remove this? What about Persistence?
        if (m_Manipulator && !((Manipulator<DataType>*)m_Manipulator)->IsValid(a_val))
            return false;

        m_Value = a_val;
        Execute(m_DataChanged);
        return true;
    }
    virtual const DataType& GetValue() { return m_Value; }
    /// Get the value as a human readable string.
    virtual std::string GetValueString()
    {
        std::ostringstream s;
        s << m_Value;
        return s.str();
    }

    HmiItem::Type GetType() const override { return HmiItem::EData; }

private:
    DataType m_Value;
};


typedef TData<std::string> Note;


#endif // HAND_GRAPH_DATA_H
