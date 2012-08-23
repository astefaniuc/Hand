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


#ifndef HAND_FUNCTOIDSEARCH_H
#define HAND_FUNCTOIDSEARCH_H

#include "graph/functoidlist.h"
#include "graph/search/searchexpression.h"


using namespace std;


class SearchCookie;
class Pool;

class FunctoidSearch : public FunctoidList
{
    public:
        FunctoidSearch();
        virtual ~FunctoidSearch();
    protected:
        virtual void InitVars();
    public:
        void CleanUp();
        void ClearFindings();
        // Search setters and getters
        // The setters are called from "outside", from the application code
        void SetSearchName(string s, bool make_regex=false);
        // The getters are called from inside, the Find methods
        SearchExpression* GetSearchName();
        void SetSearchType(string s, bool make_regex=false);
        SearchExpression* GetSearchType();
        void SetSearchRelation(string s, bool make_regex=false);
        SearchExpression* GetSearchRelation();
        virtual string GetCookieName();
        virtual bool Search(Functoid* target);
    protected:
        virtual bool Matches(Functoid* target);
        bool Step(SearchCookie* path);
        bool SearchAllChilds(SearchCookie* trail_end);
        void ExtendPath(Functoid* tree_node, SearchCookie* path_end);
        bool MarkDeathBranch(SearchCookie* branch);
        void DecomposeDeathBranches(SearchCookie* branch);
        SearchExpression* AddSearchExpression(string name, string s, bool make_regex);
    public:
        // Result setter and getter
        void AddFinding(Functoid* finding);
        Functoid* GetFindings();
    protected:
        // Quick links
        SearchExpression* SearchName;
        SearchExpression* SearchType;
        SearchExpression* SearchRelation;
        Pool* CookiePool;
    protected:
        Functoid* Findings;
    public:
        // TODO: GUI display?
        uint MaxDepth;
        bool MultipleFinds;
        bool RemoveDeadBranch;
};

#endif // HAND_FUNCTOIDSEARCH_H
