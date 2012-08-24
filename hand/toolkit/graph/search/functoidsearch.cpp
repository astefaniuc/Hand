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

#include "graph/search/functoidsearch.h"
#include "graph/search/searchcookie.h"
#include "graph/search/searchstring.h"


using namespace std;


VertexSearch::VertexSearch() : List("VertexSearch")
{
    InitVars();
    CookiePool = new Pool();
}


VertexSearch::~VertexSearch()
{
    delete(CookiePool);
    ClearFindings();
}


void VertexSearch::InitVars()
{
    SearchName = NULL;
    SearchType = NULL;
    SearchRelation = NULL;
    Findings = NULL;
    MaxDepth = MAX_SEARCH_DEPTH;
    RemoveDeadBranch = true;
    // Todo:
    MultipleFinds = false;
}


void VertexSearch::Reset()
{
    ClearFindings();
    InitVars();
}


void VertexSearch::ClearFindings()
{
    if(!Findings) return;
    // Don't call destructor for the findings from container
    if(MultipleFinds)
        Findings->clear();
    else
        Detach(Findings);
}


void VertexSearch::SetSearchName(string s, bool make_regex)
{
    // Add a link to the own interface for the GUI
    SearchName = AddSearchExpression("SearchName", s, make_regex);
}


SearchExpression* VertexSearch::GetSearchName()
{
    return SearchName;
}


void VertexSearch::SetSearchType(string s, bool make_regex)
{
    // Add a link to the own interface for the GUI
    SearchType = AddSearchExpression("SearchType", s, make_regex);
}


SearchExpression* VertexSearch::GetSearchType()
{
    return SearchType;
}


void VertexSearch::SetSearchRelation(string s, bool make_regex)
{
    // Add a link to the own interface for the GUI
    SearchRelation = AddSearchExpression("SearchRelation", s, make_regex);
}


SearchExpression* VertexSearch::GetSearchRelation()
{
    return SearchRelation;
}


SearchExpression* VertexSearch::AddSearchExpression(string relation_name, string s, bool make_regex)
{
    SearchExpression* se;
    if(make_regex)
        se = new SearchRegex(s);
    else
        se = new SearchString(s);

    // Add to the own interface for the GUI (only one entry allowed)
    Get(relation_name)->Set(se);
    return se;
}


void VertexSearch::AddFinding(Vertex* finding)
{
    if(MultipleFinds)
    {
        if(!Findings)
        {
            Findings = new List("Findings");
            Add(Findings);
        }
        Findings->Add(finding);
    }
    else
        Findings = finding;
}


Vertex* VertexSearch::GetFindings()
{
    return Findings;
}


string VertexSearch::GetCookieName()
{
    return SEARCHCOOKIE;
}


bool VertexSearch::Search(Vertex* target)
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

    SearchCookie* path = (SearchCookie*)(CookiePool->Get());
    target->Attach(path);
    path->Target = target;

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
        if(RemoveDeadBranch)
            DecomposeDeathBranches(path);
    }
    delete(path);
    return found;
}


bool VertexSearch::Step(SearchCookie* path)
{
    bool found = false;
    if(path->size()>1)
    {
        SearchCookie* branch;
        uint i = 0;
        while((branch=(SearchCookie*)path->Get(++i)) != NULL)
        {
            if((!branch->IsDeadBranch) && Step(branch))
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
        // Is this branch dead?
        MarkDeathBranch(path);
    }
    return found;
}


bool VertexSearch::SearchAllChilds(SearchCookie* path_end)
{
    bool found = false;
    Vertex* child;
    uint i = 0;
    while((child=path_end->Target->Get(++i)) != NULL)
    {
        if(!child->IsOpen(this))
            continue;
        if(Matches(child))
        {
            if(!MultipleFinds)
                return true;
            found = true;
        }
        ExtendPath(child, path_end);
    }

    return found;
}


void VertexSearch::ExtendPath(Vertex* tree_node, SearchCookie* path_end)
{
    if(!tree_node)
        return;

    Vertex* path_extension = CookiePool->Get();
    tree_node->Attach(path_extension);
    ((SearchCookie*)path_extension)->Target = tree_node;
    path_end->Add(path_extension);
}


bool VertexSearch::Matches(Vertex* target)
{
    // Ignore relation_type here (it's checked in the "Relation" Vertex)
    if(SearchName && (!SearchName->Matches(target->GetName())))
        return false;

    if(SearchType && (!target->IsType(SearchType)))
        return false;

    AddFinding(target);
    return true;
}


bool VertexSearch::MarkDeathBranch(SearchCookie* branch)
{
    if(!branch)
        return false;

    VertexIterator b = branch->begin();
    VertexIterator _end = branch->end();
    if(b != _end)
        b++;
    while(b != _end)
    {
        if(!((SearchCookie*)(*b))->IsDeadBranch)
            return false;
        b++;
    }
    branch->IsDeadBranch = true;

    MarkDeathBranch(branch->Parent);

    return true;
}


void VertexSearch::DecomposeDeathBranches(SearchCookie* path)
{
    SearchCookie* branch;
    VertexIterator curr = path->begin();
    VertexIterator _end = path->end();
    if(curr != _end)
        curr++;
    while(curr != _end)
    {
        branch = (SearchCookie*)(*curr);
        if(branch->IsDeadBranch)
        {
            CookiePool->Take(branch);
            path->erase(curr);
        }
        else
        {
            DecomposeDeathBranches(branch);
            curr++;
        }
    }
}
