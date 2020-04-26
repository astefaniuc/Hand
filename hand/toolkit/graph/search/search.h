#ifndef HAND_GRAPH_SEARCH_H
#define HAND_GRAPH_SEARCH_H

#include "graph/vertex.h"
#include "graph/search/regularexpression.h"


class SearchCookie;
class Pool;

class Search : public Vertex
{
public:
    Search(const std::string& name);
    virtual ~Search();

    // Overloaded base class methods
    void reset();
    bool execute(Vertex* target);

    // Search setters and getters
    // The setters are called from "outside", from the application code
    void SetSearchname(const std::string& s, bool make_regex=false);
    // The getters are called from inside, the Find methods
    RegularExpression* GetSearchName() { return SearchName; }

    void SetSearchType(const std::string& s, bool make_regex=false);
    RegularExpression* GetSearchType() { return SearchType; }

    void SetSearchLink(const std::string& s, bool make_regex=false);
    RegularExpression* GetSearchLink() { return SearchLink; }

    std::string GetCookieName();

    // TODO: GUI display?
    uint MaxDepth = MAX_SEARCH_DEPTH;
    bool MultipleFinds = false;

protected:
    bool Matches(Vertex* target);
    bool Step(Vertex* path);
    bool SearchAllChilds(Vertex* path_end);
    RegularExpression* AddSearchRegex(
        const std::string& name, const std::string& s, bool make_regex);

private:
    // Quick links
    RegularExpression* SearchName = nullptr;
    RegularExpression* SearchType = nullptr;
    RegularExpression* SearchLink = nullptr;
    Vertex* Findings;
};

#endif // HAND_GRAPH_SEARCH_H
