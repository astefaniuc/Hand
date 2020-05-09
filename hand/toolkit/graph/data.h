#ifndef HAND_GRAPH_DATA_H
#define HAND_GRAPH_DATA_H

#include "graph/vertex.h"


class IManipulator
{
public:
    virtual ~IManipulator() = default;
    virtual ICallback* GetHmi() = 0;
};


template <typename DataType>
class Manipulator : public IManipulator
{
public:
    // TEMP?
    virtual bool IsValid(DataType) = 0;
};


class Data : public HmiItem
{
public:
    Data(const std::string& a_name, const std::string& a_description, IManipulator* a_editor)
        : HmiItem(a_name, a_description), m_Manipulator(a_editor)
    {
        if (m_Manipulator)
            AddActivationClient(m_Manipulator->GetHmi());
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

    IManipulator* GetManipulator() { return m_Manipulator; }

protected:
    Listeners m_DataChanged;
    IManipulator* m_Manipulator;
};


template <typename DataType>
class PlainData : public Data
{
public:
    PlainData(
        const std::string& a_name,
        const std::string& a_description,
        DataType a_defaultVal,
        IManipulator* a_editor = nullptr)
        : Data(a_name, a_description, a_editor), m_Value(a_defaultVal) {}

    bool SetValue(DataType a_val)
    {
        // The manipulator should call this with valid data.
        // TODO: remove this? What about Persistence?
        if (m_Manipulator && !((Manipulator<DataType>*)m_Manipulator)->IsValid(a_val))
            return false;

        m_Value = a_val;
        Execute(m_DataChanged);
        return true;
    }
    virtual DataType GetValue() { return m_Value; }
    /// Get the value as a human readable string.
    std::string GetValueString()
    {
        std::ostringstream s;
        s << m_Value;
        return s.str();
    }

    HmiItem::Type GetType() const override { return HmiItem::EData; }

private:
    DataType m_Value;
};

#endif // HAND_GRAPH_DATA_H
