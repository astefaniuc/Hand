#ifndef HAND_GRAPH_BASELIST_H
#define HAND_GRAPH_BASELIST_H

#include <vector>


class Vertex;

typedef std::vector<Vertex*>::iterator VIterator;

class BaseList : public std::vector<Vertex*>
{
public:
    using vector::erase;
    bool erase(Vertex* v)
    {
        VIterator curr = find(v);
        if(curr != end())
        {
            erase(curr);
            return true;
        }

        return false;
    };

    VIterator find(Vertex* v)
    {
        VIterator curr;
        for(curr=begin(); curr!=end(); curr++)
            if((*curr) == v)
                break;

        return curr;
    };
};

#endif // HAND_GRAPH_BASELIST_H

