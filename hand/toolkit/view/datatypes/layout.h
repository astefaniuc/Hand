#ifndef HAND_VIEW_DATATYPES_LAYOUT_H
#define HAND_VIEW_DATATYPES_LAYOUT_H

#include "graph/collection.h"
#include "graph/method.h"


class Layout : public Collection
{
public:
    Layout(const std::string& name);

    // Implements add(sub_layout), a sub_layout is a fixed field
    bool add(Vertex* child) override;

    using Collection::get;
    // Implements get(FIELD, name) used in Layer classes to poll
    // the availability of a specific field
    Vertex* get(const std::string& type, const std::string& name) override;

    bool execute(Vertex* surface) override;
    void reset() override;
};


class FieldsContainer : public Collection
{
public:
    FieldsContainer(Vertex* parent_layout) : Collection(FIELDS)
    {
        Vertex::get(PARENT)->set(parent_layout);
    }

    using Collection::get;
    // Iterates through sub-layouts (filled fields)
    Vertex* get(unsigned item) override;
    // Adds a Vertex::Link to the parent layout to new fields
    Vertex* get(const std::string& name) override;

    // Returns the number of sub-layouts (filled fields)
    unsigned size() override;
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
