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

#ifndef GRAPH_BASELIST_H
#define GRAPH_BASELIST_H

#include <vector>


class Vertex;


typedef std::vector<Vertex*>::iterator VIterator;


class BaseList : public std::vector<Vertex*>
{
    public:
        using vector::erase;
        bool erase(Vertex* v)
        {
            VIterator curr = Find(v);
            if(curr != end())
            {
                erase(curr);
                return true;
            }

            return false;
        };

        VIterator Find(Vertex* v)
        {
            VIterator curr;
            for(curr=begin(); curr!=end(); curr++)
                if((*curr) == v)
                    break;

            return curr;
        };
};

#endif /* GRAPH_BASELIST_H */
