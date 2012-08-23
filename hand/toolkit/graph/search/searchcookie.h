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

#include "graph/functoidlist.h"


using namespace std;


#define SEARCHCOOKIE "SearchCookie"

class FunctoidSearch;

class SearchCookie : public FunctoidList
{
    public:
        SearchCookie();
        virtual ~SearchCookie();
        virtual bool IsOpen(FunctoidSearch* search);
        virtual bool Add(Functoid* child);
        // Detaches cookie from Target
        virtual bool Detach(Functoid* ignore);
        Functoid* Target;
        SearchCookie* Parent;
        bool IsDeadBranch;
};


class Pool : public FunctoidList
{
    public:
        Pool() : FunctoidList("SearchCookiePool"){};
        virtual ~Pool(){};
        Functoid* Get();
        void Take(Functoid* cookie);
};

#endif /* HAND_SEARCHCOOKIE_H */
