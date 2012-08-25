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

#ifndef TOOLKIT_GRAPH_LINK_H
#define TOOLKIT_GRAPH_LINK_H

#include "graph/list.h"


// TODO: combine Link and Relation? Or even get rid of both?
class Link : public List
{
    public:
        Link(std::string name, std::string type, bool is_multi_link=false);
        virtual ~Link(){};

        virtual bool Add(Vertex* val);
        virtual bool Set(Vertex* val);
        bool Execute(Vertex* vs);
        void MakeMultiLink(bool);
        bool IsMultiLink();

   protected:
        bool IsMulti;
};


class Relation : public List
{
    public:
        Relation(std::string name);
        virtual ~Relation(){};

        // Inserts a sub-item to the current vertex without changing
        // the ownership of 'item'; it replaces *all* objects referenced
        // by the Relation (only public elements)
        virtual bool Set(Vertex* sub);
        // Helper method for the search engine
        bool IsOpen(Search* search);
};

#endif /* TOOLKIT_GRAPH_LINK_H */
