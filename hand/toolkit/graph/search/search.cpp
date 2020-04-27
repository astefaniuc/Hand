#include "graph/search/search.h"
#include "graph/search/searchcookie.h"
#include "graph/search/searchstring.h"


Search::Search(const std::string& name) : Vertex(name)
{
    type(METHOD);
    type(SEARCH);

    Findings = get("Findings");
}


Search::~Search()
{
    reset();
}


void Search::reset()
{
    Vertex* child;
    unsigned i = 0;
    while((child=Findings->get(++i)) != nullptr)
        Findings->detach(child);
}


bool Search::execute(Vertex* target)
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
    path->attach(target);

    unsigned depth;
    for(depth = 0; depth <= MaxDepth; ++depth)
    {
        if(Step(path))
        {
            found = true;
            if(!MultipleFinds)
                break;
        }

        if(!path->size())
            break;
    }

    delete(path);
    return found;
}


bool Search::Step(Vertex* path)
{
    bool found = false;
    if(!path->size())
    {
        // Head of path
        found = SearchAllChilds(path);
        if(found && !MultipleFinds)
            return true;
    }
    else
    {
        Vertex* branch;
        unsigned i = 0;
        while((branch=path->get(++i)) != nullptr)
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
    unsigned i = 0;
    while((child=path->get()->get(++i)) != nullptr)
    {
        if(!child->isOpen(this))
            continue;
        if(Matches(child))
        {
            if(!MultipleFinds)
                return true;
            found = true;
        }
        path_extension = new SearchCookie();
        path_extension->attach(child);
        path->add(path_extension);
    }

    return found;
}


bool Search::Matches(Vertex* target)
{
    // Ignore relation_type here (it's checked in the "Link" Vertex)
    if(SearchName && (!SearchName->Matches(target->name())))
        return false;

    if(SearchType && (!target->is(SearchType)))
        return false;

    Findings->attach(target);
    return true;
}


void Search::SetSearchname(const std::string& s, bool make_regex)
{
    // Add a link to the own interface for the GUI
    SearchName = AddSearchRegex("SearchName", s, make_regex);
}


void Search::SetSearchType(const std::string& s, bool make_regex)
{
    // Add a link to the own interface for the GUI
    SearchType = AddSearchRegex("SearchType", s, make_regex);
}


void Search::SetSearchLink(const std::string& s, bool make_regex)
{
    // Add a link to the own interface for the GUI
    SearchLink = AddSearchRegex("SearchLink", s, make_regex);
}


RegularExpression* Search::AddSearchRegex(
    const std::string& relation_name, const std::string& s, bool make_regex)
{
    RegularExpression* se;
    if(make_regex)
        se = new SearchRegex(s);
    else
        se = new SearchString(s);

    // Add to the own interface for the GUI (only one entry allowed)
    get(relation_name)->set(se);
    return se;
}


std::string Search::GetCookieName()
{
    return SEARCHCOOKIE;
}
