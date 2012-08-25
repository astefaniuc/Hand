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


class List : public Vertex
{
    public:
        List(std::string name);
        virtual ~List();

        // Get the child by name
        virtual Vertex* Get(std::string child);
        virtual Vertex* Get(std::string type, std::string name);
        // Get the child by position, 1-based; Element '0' stores
        // hidden system information e.g. type and layout.
        virtual Vertex* Get(uint child);

        virtual void SetType(std::string type);
        virtual std::string GetType();
        virtual bool IsType(std::string type);
        virtual bool IsType(RegularExpression* type);

        // Set object owner (for memory management)
        virtual void SetOwner(Vertex* owner);
        virtual bool HasOwner(Vertex* caller);

        // Remove all public children
        virtual void Reset();
};

#endif /* TOOLKIT_GRAPH_LIST_H */
