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

#ifndef GRAPH_SEARCH_SEARCHCOOKIE_H
#define GRAPH_SEARCH_SEARCHCOOKIE_H

#include "graph/list.h"


#define SEARCHCOOKIE "SearchCookie"

class Search;

class SearchCookie : public List
{
    public:
        SearchCookie();
        virtual ~SearchCookie();

        // Sets Target
        bool attach(Vertex* target);
        // Returns open sub-paths
        Vertex* get(uint item);
        // Returns always 'false'
        bool isOpen(Search* search);
    protected:
        // Overloaded to return the Target
        Vertex* _get();
    private:
        bool IsDeadBranch;
        Vertex* Target;
};

#endif /* GRAPH_SEARCH_SEARCHCOOKIE_H */
