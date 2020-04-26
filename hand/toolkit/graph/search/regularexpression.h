#ifndef HAND_GRAPH_SEARCH_REGULAREXPRESSION_H
#define HAND_GRAPH_SEARCH_REGULAREXPRESSION_H

#include "graph/vertex.h"


class RegularExpression : public Vertex
{
public:
    RegularExpression(const std::string& name) : Vertex(name) {}
    virtual bool Matches(const std::string& s) = 0;
};


#endif // HAND_GRAPH_SEARCH_REGULAREXPRESSION_H
