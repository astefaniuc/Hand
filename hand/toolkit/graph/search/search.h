/*
 *  Copyright 2012 Alex Stefaniuc
 *
 *  This file is part of Hand.
 *
 *  Hand is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License
 *  as published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  Hand is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with Hand. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef GRAPH_SEARCH_H
#define GRAPH_SEARCH_H

#include "graph/list.h"
#include "graph/search/regularexpression.h"


class SearchCookie;
class Pool;

class Search : public List
{
    public:
        Search();
        virtual ~Search();
    protected:
        virtual void InitVars();
    public:
        void Reset();
        void ClearFindings();
        // Search setters and getters
        // The setters are called from "outside", from the application code
        void SetSearchName(std::string s, bool make_regex=false);
        // The getters are called from inside, the Find methods
        RegularExpression* GetSearchName();
        void SetSearchType(std::string s, bool make_regex=false);
        RegularExpression* GetSearchType();
        void SetSearchRelation(std::string s, bool make_regex=false);
        RegularExpression* GetSearchRelation();
        virtual std::string GetCookieName();
        virtual bool Execute(Vertex* target);
    protected:
        virtual bool Matches(Vertex* target);
        bool Step(SearchCookie* path);
        bool SearchAllChilds(SearchCookie* trail_end);
        void ExtendPath(Vertex* tree_node, SearchCookie* path_end);
        bool MarkDeathBranch(SearchCookie* branch);
        void DecomposeDeathBranches(SearchCookie* branch);
        RegularExpression* AddSearchRegex(std::string name,
                                          std::string s,
                                          bool make_regex);
    public:
        // Result setter and getter
        void AddFinding(Vertex* finding);
        Vertex* GetFindings();
    protected:
        // Quick links
        RegularExpression* SearchName;
        RegularExpression* SearchType;
        RegularExpression* SearchRelation;
        Pool* CookiePool;
    protected:
        Vertex* Findings;
    public:
        // TODO: GUI display?
        uint MaxDepth;
        bool MultipleFinds;
        bool RemoveDeadBranch;
};

#endif // GRAPH_SEARCH_H
