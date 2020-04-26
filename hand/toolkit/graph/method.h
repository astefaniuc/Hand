#ifndef HAND_GRAPH_METHOD_H
#define HAND_GRAPH_METHOD_H

#include "graph/vertex.h"


template <class I>
class Method : public Vertex
{
typedef bool (I::*TFunction)(Vertex*);

public:
    Method(std::string name, I* obj, TFunction func)
        : Vertex(name), Object(obj), Function(func) { type(METHOD); }

    // Execute the Method
    bool execute(Vertex* param)
    {
        if(Function)
            return (Object->*Function)(param);
        return false;
    }

    using Vertex::set;
    void set(TFunction func) { Function = func; }

    using Vertex::get;
    virtual I* get()
    {
        // Only makes sense to return the object
        return Object;
    }

private:
    I* Object;
    TFunction Function;
};

#endif // HAND_GRAPH_METHOD_H
