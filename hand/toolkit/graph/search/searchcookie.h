#ifndef HAND_GRAPH_SEARCH_SEARCHCOOKIE_H
#define HAND_GRAPH_SEARCH_SEARCHCOOKIE_H

#include "graph/collection.h"


#define SEARCHCOOKIE "SearchCookie"

class Search;

class SearchCookie : public Collection
{
public:
    SearchCookie() : Collection(SEARCHCOOKIE) {}
    virtual ~SearchCookie();

    // Sets Target
    bool attach(Vertex* target);
    // Returns open sub-paths
    Vertex* get(unsigned item);
    bool isOpen(Search* search) override { return false; }

protected:
    // Overloaded to return the Target
    Vertex* _get() override { return Target; }


private:
    bool IsDeadBranch = false;
    Vertex* Target = nullptr;
};

#endif // HAND_GRAPH_SEARCH_SEARCHCOOKIE_H
