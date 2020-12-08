#ifndef HAND_GRAPH_HMIITEM_H
#define HAND_GRAPH_HMIITEM_H

#include "input/chord.h"
#include <string>
#include <sstream>
#include <vector>


namespace Hmi { class Item; }

class ICallback
{
public:
    virtual ~ICallback() = default;
    virtual void Execute(Hmi::Item* caller) = 0;
};


template <class CallbackOwner>
class CCallback : public ICallback
{
    typedef void (CallbackOwner::*TCallback)(Hmi::Item*);

public:
    CCallback(CallbackOwner* obj, TCallback func)
        : m_Object(obj), m_Function(func) {}

    void Execute(Hmi::Item* caller) final
    {
        if(m_Object && m_Function)
            (m_Object->*m_Function)(caller);
    }

private:
    CallbackOwner* m_Object;
    TCallback m_Function;
};


class Layer;
class Chord;


namespace Hmi {


class List;

class Item
{
public:
    enum InfoLevel
    {
        Hint,
        Info,
        Warning,
        Error,
        Fatal
    };

    Item(const std::string& name, const std::string& description)
        : m_Name(name), m_Info(description) {}
    virtual ~Item();

    void SetName(const std::string& name) { m_Name = name; }
    const std::string& GetName() const { return m_Name; }

    const std::string& GetInfo() const { return m_Info; }
    void SetInfo(const std::string& info, InfoLevel type, bool persistent = true) {}

    /// The parent is the object owner; HmiItems without a parent have to be explicitly deleted.
    void SetParent(Hmi::List* parent) { m_Parent = parent; }
    Hmi::List* GetParent() const { return m_Parent; }


    Layer* GetExpandedView();
    void SetExpandedView(Layer* layer);

    Layer* GetButtonView();
    void SetButtonView(Layer* layer);


    void SetSelected(bool isSelected);
    bool IsSelected() const { return m_IsSelected; }
    /// The callback is executed whenever the selection changes.
    void AddSelectionClient(ICallback* client)
    {
        m_SelectionChange.push_back(client);
    }
    void RemoveSelectionClient(ICallback* client)
    {
        RemoveCallback(client, m_SelectionChange);
    }


    void Activate() { Execute(m_Activation); }
    void AddActivationClient(ICallback* client)
    {
        m_Activation.push_back(client);
    }
    void RemoveActivationClient(ICallback* client)
    {
        RemoveCallback(client, m_Activation);
    }

    // Layout options

    /// You can keep HmiItems in the closer focus while the user descends into details.
    void SetVisualKeepLevel(int level);

    Chord m_Chord;

protected:
    typedef std::vector<ICallback*> Listeners;

    void RemoveCallback(ICallback* client, Listeners& clientsList);
    void Execute(const Listeners& list);

    virtual Layer* CreateExpandedView() = 0;
    virtual Layer* CreateButtonView();

private:
    std::string m_Name;
    std::string m_Info;

    Hmi::List* m_Parent = nullptr;
    Layer* m_ExpandedView = nullptr;
    Layer* m_ButtonView = nullptr;
    bool m_IsSelected = false;

    Listeners m_SelectionChange;
    Listeners m_Activation;
};

}
#endif // HAND_GRAPH_HMIITEM_H