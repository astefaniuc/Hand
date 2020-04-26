#ifndef HAND_GRAPH_SEARCH_SEARCHCOOKIE_H
#define HAND_GRAPH_SEARCH_SEARCHCOOKIE_H

#include "graph/list.h"


#define SEARCHCOOKIE "SearchCookie"

class Search;

class SearchCookie : public List
{
public:
    SearchCookie() : List(SEARCHCOOKIE) {}
    virtual ~SearchCookie();

    // Sets Target
    bool attach(Vertex* target);
    // Returns open sub-paths
    Vertex* get(uint item);
    bool isOpen(Search* search) override { return false; }

protected:
    // Overloaded to return the Target
    Vertex* _get() override { return Target; }


private:
    bool IsDeadBranch = false;
    Vertex* Target = nullptr;
};

#endif // HAND_GRAPH_SEARCH_SEARCHCOOKIE_H
