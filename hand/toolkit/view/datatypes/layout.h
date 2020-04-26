#ifndef HAND_VIEW_DATATYPES_LAYOUT_H
#define HAND_VIEW_DATATYPES_LAYOUT_H

#include "graph/list.h"
#include "graph/method.h"


class Layout : public List
{
public:
    Layout(const std::string& name);

    // Implements add(sub_layout), a sub_layout is a fixed field
    bool add(Vertex* child) override;

    using List::get;
    // Implements get(FIELD, name) used in Layer classes to poll
    // the availability of a specific field
    Vertex* get(const std::string& type, const std::string& name) override;

    bool execute(Vertex* surface) override;
    void reset() override;
};


class FieldsContainer : public List
{
public:
    FieldsContainer(Vertex* parent_layout) : List(FIELDS)
    {
        Vertex::get(PARENT)->set(parent_layout);
    }

    using List::get;
    // Iterates through sub-layouts (filled fields)
    Vertex* get(uint item) override;
    // Adds a Vertex::Link to the parent layout to new fields
    Vertex* get(const std::string& name) override;

    // Returns the number of sub-layouts (filled fields)
    uint size() override;
};


template <class I>
class LayoutFactory : public Method<I>
{
    typedef bool (I::*TFunction)(Vertex*);

public:
    LayoutFactory(const std::string& name, I* obj, TFunction func) : Method<I>(name, obj, func)
    {
        Vertex::type(FACTORY);
    }

    Vertex* _get() override
    {
        Vertex* ret = new Layout(Vertex::name());
        Vertex::add(ret);
        Method<I>::execute(ret);
        return ret;
    }
};

#endif // HAND_VIEW_DATATYPES_LAYOUT_H
