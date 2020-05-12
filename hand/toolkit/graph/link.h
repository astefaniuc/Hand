#ifndef HAND_GRAPH_LINK_H
#define HAND_GRAPH_LINK_H

#include "graph/collection.h"


class Link : public Collection
{
public:
    Link(const std::string& name) : Collection(name) { type(LINK); }

    // Helper method for the search engine
    bool isOpen(Search* search);
};

#endif // HAND_GRAPH_LINK_H
