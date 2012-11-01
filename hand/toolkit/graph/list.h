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

#ifndef GRAPH_LIST_H
#define GRAPH_LIST_H

#include "graph/vertex.h"


// Keeps a list of "public" elements in a clean(*) child vertex
// under PUBLIC.
//
// (*) no type or runtime info
//
class List : public Vertex
{
    public:
        List(std::string name);
        virtual ~List(){};

        // Overloads of the public interface of the Vertex class
        // to operate on the list elements
        virtual bool add(Vertex* child);
        virtual bool set(Vertex* sub);
        virtual bool attach(Vertex* sub);
        using Vertex::get;
        virtual Vertex* get(std::string child);
        virtual Vertex* get(std::string type, std::string name);
        virtual Vertex* get(uint child);
        virtual uint size();
        // Removes all entries from the PUBLIC sub-vertex
        virtual void reset();
        virtual bool detach(Vertex* item);
        virtual bool remove(Vertex* child);
    protected:
        virtual Vertex* _get();
};

#endif /* GRAPH_LIST_H */
