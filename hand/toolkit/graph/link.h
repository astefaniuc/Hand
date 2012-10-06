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

#ifndef GRAPH_LINK_H
#define GRAPH_LINK_H

#include "graph/list.h"


class Relation : public List
{
    public:
        Relation(std::string name);
        virtual ~Relation(){};

        // Inserts a sub-item to the current vertex without changing
        // the ownership of 'item'; it replaces *all* (public) objects
        // referenced by the Relation
        virtual bool Set(Vertex* target);
        // Helper method for the search engine
        bool IsOpen(Search* search);
};

#endif /* GRAPH_LINK_H */
