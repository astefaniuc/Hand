#include "graph/search/searchcookie.h"
#include "graph/search/search.h"


SearchCookie::~SearchCookie()
{
    if(Target)
        Target->Vertex::detach(this);
}


bool SearchCookie::attach(Vertex* target)
{
    if(!target->size())
    {
        IsDeadBranch = true;
        return false;
    }
    Target = target;
    return Target->Vertex::attach(this);
}


Vertex* SearchCookie::get(unsigned i)
{
    SearchCookie* branch = (SearchCookie*)List::get(i);
    if(branch && branch->IsDeadBranch)
    {
        delete(branch);

        branch = (SearchCookie*)SearchCookie::get(i);
        if(!branch && (i == 1))
            IsDeadBranch = true;
    }
    return branch;
}
