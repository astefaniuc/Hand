#include "graph/link.h"
#include "graph/search/search.h"


bool Link::isOpen(Search* search)
{
    if(!Vertex::isOpen(search))
        return false;
    // Does it match in its role as relation
    RegularExpression* se = search->GetSearchLink();
    if(se && !se->Matches(name()))
        // Don't look further if it's not the right relation
        return false;
    return true;
}
