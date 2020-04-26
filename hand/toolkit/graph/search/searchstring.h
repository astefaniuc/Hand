#ifndef HAND_GRAPH_SEARCH_SEARCHSTRING_H
#define HAND_GRAPH_SEARCH_SEARCHSTRING_H

#include <regex>
#include "graph/search/regularexpression.h"


class SearchString : public RegularExpression
{
public:
    SearchString(const std::string& name) : RegularExpression(name) {}
    bool Matches(const std::string& s) override { return (name() == s); }
};


class SearchRegex : public RegularExpression
{
public:
    SearchRegex(const std::string& name) : RegularExpression(name), RegEx(name) {}
    bool Matches(const std::string& s) override { return regex_match(s, RegEx); }

protected:
    std::regex RegEx;
};


#endif // HAND_GRAPH_SEARCH_SEARCHSTRING_H
