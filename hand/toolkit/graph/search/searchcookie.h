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

#ifndef HAND_SEARCHCOOKIE_H
#define HAND_SEARCHCOOKIE_H

#include "graph/list.h"


#define SEARCHCOOKIE "SearchCookie"

class VertexSearch;

class SearchCookie : public List
{
    public:
        SearchCookie();
        virtual ~SearchCookie();
        virtual bool IsOpen(VertexSearch* search);
        virtual bool Add(Vertex* child);
        // Detaches cookie from Target
        virtual bool Detach(Vertex* ignore);
        Vertex* Target;
        SearchCookie* Parent;
        bool IsDeadBranch;
};


class Pool : public List
{
    public:
        Pool() : List("SearchCookiePool"){};
        virtual ~Pool(){};
        Vertex* Get();
        void Take(Vertex* cookie);
};

#endif /* HAND_SEARCHCOOKIE_H */
