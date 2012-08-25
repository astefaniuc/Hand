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

#ifndef TOOLKIT_GRAPH_LIST_H
#define TOOLKIT_GRAPH_LIST_H

#include "graph/vertex.h"


// Keeps a list of "public" elements in a clean(*) child vertex
// under PUBLICLIST.
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
        virtual bool Add(Vertex* child);
        virtual bool Set(Vertex* sub);
        virtual bool Attach(Vertex* sub);
        virtual Vertex* Get(std::string child);
        virtual Vertex* Get(std::string type, std::string name);
        virtual Vertex* Get(uint child);
        // Removes the complete PUBLICLIST vertex
        virtual void Reset();
};

#endif /* TOOLKIT_GRAPH_LIST_H */
