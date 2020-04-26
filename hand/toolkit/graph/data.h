#ifndef HAND_GRAPH_DATA_H
#define HAND_GRAPH_DATA_H

#include <typeinfo>
#include "graph/vertex.h"


template <typename I>
class Data : public Vertex
{
public:
    Data(const std::string& name, I val) : Vertex(name), Value(val)
    {
        type(DATA);
        type(std::string(typeid(val).name()));
    }

    using Vertex::set;
    bool set(I val)
    {
        Value = val;
        return true;
    }

    using Vertex::get;
    virtual I get()
    {
        return Value;
    }
    std::string getAsString()
    {
        std::ostringstream s;
        s << Value;
        return s.str();
    }

protected:
    I Value;
};

typedef Data<std::string> Note;

#endif // HAND_GRAPH_DATA_H
