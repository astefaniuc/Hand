#ifndef HAND_GRAPH_VERTEX_H
#define HAND_GRAPH_VERTEX_H

#include <string>
#include <sstream>
#include <vector>
#include "include/defines.h"


class BaseList;
class Search;
class RegularExpression;

class Vertex
{
public:
    Vertex(const std::string& name);
    virtual ~Vertex();

    // Appends (if not already there) a sub-item to the current vertex
    // and makes the current vertex the owner of it
     virtual bool add(Vertex* child);
    // Inserts (or moves) a sub-item into the current vertex at first position
    // ("default" value returned by get()) without changing the ownership of 'sub'
    virtual bool set(Vertex* sub);
    // Appends (if not already there) a sub-item to the current vertex
    // without changing the ownership of 'sub'
    virtual bool attach(Vertex* sub);

    // Return the default child/value which is nullptr for a plain vertex
    Vertex* get();
    // Get the first sub-item by name.
    // If the item doesn't exist it returns a new Link vertex;
    // use get(ANY, name) to test if a vertex exists
    virtual Vertex* get(const std::string& name);
    // Get the first sub-item by name and type; if the item is not found
    // it returns nullptr; the parameters can be set to ANY to ignore that value;
    // get(ANY, ANY) ignores the type but does search for item named ANY ("*")
    virtual Vertex* get(const std::string& type, const std::string& name);
    // Get the child by position, 1-based
    virtual Vertex* get(unsigned item);

    // Get the number of elements
    virtual unsigned size();

    // Simple iterative deepening depth-first search.
    // Omits OWNER from the search but might fail with any other graph
    // cycles, use the Search engine in case of doubt.
    virtual Vertex* find(const std::string& name, int max_depth = 2);
    // Flat search with support for (boost-typed) regular expressions
    // TODO: make it iterative or rename to it "get()"?
    virtual Vertex* find(RegularExpression* expression);

    // Set Vertex name
    void name(const std::string& name){ Name = name; }
    // Get the Vertex name
    const std::string& name() { return Name; }


    // Set the Vertex type/tag
    void type(const std::string& type);
    // Returns the last set type
    std::string type();
    bool is(const std::string& type);
    bool is(RegularExpression* type);

    // Removes all sub-objects
    virtual void reset();
    // Removes child from Vertex. If Vertex is the owner of child it also
    // resets the ownership
    virtual bool detach(Vertex* child);
    // Removes the reference to 'item' and deletes the object if it
    // is the owner
    virtual bool remove(Vertex* child);

    // Returns the string to be visualized, here: the name
    virtual std::string getAsString() { return Name; }

    // Interface to methods defined in a derived class
    virtual bool execute(Vertex* func_param) { return false; }

    // Helper method for the search engine
    virtual bool isOpen(Search* search);

protected:
    // Helper method allowing to virtualize the return value of get(void);
    // overload this if you want to return a different default value
    virtual Vertex* _get();
    virtual Vertex* _find(const std::string& name, int depth);

private:
    // Set object owner (for memory management).
    void setOwner(Vertex* owner);
    // Returns the pointer to the owner vertex
    Vertex* getOwner();

    std::string Name;
    BaseList* Body;
    BaseList* References;
    Vertex* Type = nullptr;
};


class HmiItem;

class ICallback
{
public:
    virtual ~ICallback() = default;
    virtual HmiItem* Execute(HmiItem* a_caller) = 0;
};


template <class CallbackOwner>
class CCallback : public ICallback
{
    typedef void (CallbackOwner::*TCallback)(HmiItem*);

public:
    CCallback(CallbackOwner* a_obj, TCallback a_func)
        : m_Object(a_obj), m_Function(a_func) {}

    HmiItem* Execute(HmiItem* a_caller) final
    {
        if(m_Object && m_Function)
            return (m_Object->*m_Function)(a_caller);
        return a_caller;
    }

private:
    CallbackOwner* m_Object;
    TCallback m_Function;
};


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

    const std::string& GetName() const { return m_Name; }
    const std::string& GetDescription() const { return m_Description; }
    virtual Type GetType() const { return EBase; }

    /// The parent is the object owner; HmiItems without a parent have to be explicitly deleted.
    void SetParent(HmiItem* a_parent) { m_Parent = a_parent; }
    HmiItem* GetParent() const { return m_Parent; }


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
    bool m_IsSelected = false;

    Listeners m_SelectionChange;
    Listeners m_Activation;
};

#endif // HAND_GRAPH_VERTEX_H
