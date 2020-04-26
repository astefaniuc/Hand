#ifndef HAND_GRAPH_LINK_H
#define HAND_GRAPH_LINK_H

#include "graph/list.h"


class Link : public List
{
public:
    Link(const std::string& name) : List(name) { type(LINK); }

    // Helper method for the search engine
    bool isOpen(Search* search);
};

#endif // HAND_GRAPH_LINK_H
