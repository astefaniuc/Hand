#ifndef HAND_GRAPH_VERTEX_H
#define HAND_GRAPH_VERTEX_H

#include <string>
#include <sstream>
#include <vector>
#include "include/defines.h"


class HmiItem;

class ICallback
{
public:
    virtual ~ICallback() = default;
    virtual void Execute(HmiItem* a_caller) = 0;
};


template <class CallbackOwner>
class CCallback : public ICallback
{
    typedef void (CallbackOwner::*TCallback)(HmiItem*);

public:
    CCallback(CallbackOwner* a_obj, TCallback a_func)
        : m_Object(a_obj), m_Function(a_func) {}

    void Execute(HmiItem* a_caller) final
    {
        if(m_Object && m_Function)
            (m_Object->*m_Function)(a_caller);
    }

private:
    CallbackOwner* m_Object;
    TCallback m_Function;
};


class Layer;

class HmiItem
{
public:
    enum Type
    {
        EBase,
        EAction,
        EData,
        EList
    };

    HmiItem(const std::string& a_name, const std::string& a_description)
        : m_Name(a_name),
          m_Description(a_description)
    {}

    virtual ~HmiItem() = default;

    void SetName(const std::string& a_name) { m_Name = a_name; }
    const std::string& GetName() const { return m_Name; }

    const std::string& GetDescription() const { return m_Description; }
    virtual Type GetType() const { return EBase; }

    /// The parent is the object owner; HmiItems without a parent have to be explicitly deleted.
    void SetParent(HmiItem* a_parent) { m_Parent = a_parent; }
    HmiItem* GetParent() const { return m_Parent; }

    Layer* GetLayer();
    void SetLayer(Layer* visualization);

    void SetSelected(bool a_isSelected);
    bool IsSelected() const { return m_IsSelected; }
    /// The callback is executed whenever the selection changes.
    void AddSelectionClient(ICallback* a_client)
    {
        m_SelectionChange.push_back(a_client);
    }
    void RemoveSelectionClient(ICallback* a_client)
    {
        RemoveCallback(a_client, m_SelectionChange);
    }


    void Activate() { Execute(m_Activation); }
    void AddActivationClient(ICallback* a_client)
    {
        m_Activation.push_back(a_client);
    }
    void RemoveActivationClient(ICallback* a_client)
    {
        RemoveCallback(a_client, m_Activation);
    }

    // Layout options

    /// You can keep HmiItems in the closer focus while the user descends into details.
    void SetVisualPersistence(int a_level);

protected:
    typedef std::vector<ICallback*> Listeners;

    void RemoveCallback(ICallback* a_client, Listeners& a_clientsList);
    void Execute(const Listeners& a_list);

private:
    std::string m_Name;
    std::string m_Description;

    HmiItem* m_Parent = nullptr;
    Layer* m_Visualization = nullptr;
    bool m_IsSelected = false;

    Listeners m_SelectionChange;
    Listeners m_Activation;
};

#endif // HAND_GRAPH_VERTEX_H
