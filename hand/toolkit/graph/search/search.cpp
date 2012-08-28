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

#include "graph/search/search.h"
#include "graph/search/searchcookie.h"
#include "graph/search/searchstring.h"


using namespace std;


Search::Search(string name) : Vertex(name)
{
    SetType(METHOD);
    SetType(SEARCH);

    Findings = Get("Findings");
    SearchName = NULL;
    SearchType = NULL;
    SearchRelation = NULL;
    MaxDepth = MAX_SEARCH_DEPTH;
    RemoveDeadBranch = false;
    MultipleFinds = false;

    CookiePool = new Pool();
}


Search::~Search()
{
    delete(CookiePool);
    Reset();
}


void Search::Reset()
{
    Vertex* child;
    uint i = 0;
    while((child=Findings->Get(++i)) != NULL)
        Findings->Detach(child);
}


bool Search::Execute(Vertex* target)
{
    if(!target)
        return false;
    bool found = false;

    if(Matches(target))
    {
        if(!MultipleFinds)
            return true;
        found = true;
    }

    SearchCookie* path = (SearchCookie*)BuildPath(target);

    uint depth;
    for(depth=0; depth<=MaxDepth; depth++)
    {
        if(Step(path))
        {
            found = true;
            if(!MultipleFinds)
                break;
        }
        if(path->IsDeadBranch)
            break;
    }
    CookiePool->Take(path);
    return found;
}


bool Search::Step(SearchCookie* path)
{
    bool found = false;
    if(path->GetSize() > 0)
    {
        SearchCookie* branch;
        uint i = 0;
        while((branch=(SearchCookie*)path->Get(++i)) != NULL)
        {
            if(branch->IsDeadBranch && RemoveDeadBranch)
            {
                path->Detach(branch);
                CookiePool->Take(branch);
            }
            else if(Step(branch))
            {
                found = true;
                if(!MultipleFinds)
                    break;
            }
        }
    }
    else
    {
        // Head of path
        found = SearchAllChilds(path);
        if(found && !MultipleFinds)
            return true;

        // Is this branch dead?
        path->MarkDeathBranch();
    }
    return found;
}


bool Search::SearchAllChilds(SearchCookie* path)
{
    bool found = false;
    Vertex* child;
    uint i = 0;
    while((child=path->Target->Get(++i)) != NULL)
    {
        if(!child->IsOpen(this))
            continue;
        if(Matches(child))
        {
            if(!MultipleFinds)
                return true;
            found = true;
        }
        path->Add(BuildPath(child));
    }

    return found;
}


Vertex* Search::BuildPath(Vertex* target)
{
    if(!target)
        return NULL;

    Vertex* path_extension = CookiePool->Get();
    ((SearchCookie*)path_extension)->Target = target;
    target->Vertex::Attach(path_extension);

    return path_extension;
}


bool Search::Matches(Vertex* target)
{
    // Ignore relation_type here (it's checked in the "Relation" Vertex)
    if(SearchName && (!SearchName->Matches(target->GetName())))
        return false;

    if(SearchType && (!target->IsType(SearchType)))
        return false;

    Findings->Attach(target);
    return true;
}


void Search::SetSearchName(string s, bool make_regex)
{
    // Add a link to the own interface for the GUI
    SearchName = AddSearchRegex("SearchName", s, make_regex);
}


RegularExpression* Search::GetSearchName()
{
    return SearchName;
}


void Search::SetSearchType(string s, bool make_regex)
{
    // Add a link to the own interface for the GUI
    SearchType = AddSearchRegex("SearchType", s, make_regex);
}


RegularExpression* Search::GetSearchType()
{
    return SearchType;
}


void Search::SetSearchRelation(string s, bool make_regex)
{
    // Add a link to the own interface for the GUI
    SearchRelation = AddSearchRegex("SearchRelation", s, make_regex);
}


RegularExpression* Search::GetSearchRelation()
{
    return SearchRelation;
}


RegularExpression* Search::AddSearchRegex(string relation_name, string s, bool make_regex)
{
    RegularExpression* se;
    if(make_regex)
        se = new SearchRegex(s);
    else
        se = new SearchString(s);

    // Add to the own interface for the GUI (only one entry allowed)
    Get(relation_name)->Set(se);
    return se;
}


string Search::GetCookieName()
{
    return SEARCHCOOKIE;
}
