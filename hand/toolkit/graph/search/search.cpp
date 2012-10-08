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
    Type(METHOD);
    Type(SEARCH);

    Findings = Get("Findings");
    SearchName = NULL;
    SearchType = NULL;
    SearchLink = NULL;
    MaxDepth = MAX_SEARCH_DEPTH;
    MultipleFinds = false;
}


Search::~Search()
{
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

    Vertex* path = new SearchCookie();
    path->Attach(target);

    uint depth;
    for(depth=0; depth<=MaxDepth; depth++)
    {
        if(Step(path))
        {
            found = true;
            if(!MultipleFinds)
                break;
        }

        if(path->Size() == 0)
            break;
    }

    delete(path);
    return found;
}


bool Search::Step(Vertex* path)
{
    bool found = false;
    if(path->Size() == 0)
    {
        // Head of path
        found = SearchAllChilds(path);
        if(found && !MultipleFinds)
            return true;
    }
    else
    {
        Vertex* branch;
        uint i = 0;
        while((branch=path->Get(++i)) != NULL)
        {
            if(Step(branch))
            {
                found = true;
                if(!MultipleFinds)
                    break;
            }
        }
    }
    return found;
}


bool Search::SearchAllChilds(Vertex* path)
{
    bool found = false;
    Vertex* child;
    Vertex* path_extension;
    uint i = 0;
    while((child=path->Get()->Get(++i)) != NULL)
    {
        if(!child->IsOpen(this))
            continue;
        if(Matches(child))
        {
            if(!MultipleFinds)
                return true;
            found = true;
        }
        path_extension = new SearchCookie();
        path_extension->Attach(child);
        path->Add(path_extension);
    }

    return found;
}


bool Search::Matches(Vertex* target)
{
    // Ignore relation_type here (it's checked in the "Link" Vertex)
    if(SearchName && (!SearchName->Matches(target->Name())))
        return false;

    if(SearchType && (!target->Is(SearchType)))
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


void Search::SetSearchLink(string s, bool make_regex)
{
    // Add a link to the own interface for the GUI
    SearchLink = AddSearchRegex("SearchLink", s, make_regex);
}


RegularExpression* Search::GetSearchLink()
{
    return SearchLink;
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
