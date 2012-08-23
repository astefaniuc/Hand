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

#ifndef HAND_SEARCHSTRING_H
#define HAND_SEARCHSTRING_H

#include "graph/search/searchexpression.h"

#include <regex>


class SearchString : public SearchExpression
{
    public:
        SearchString(std::string name) : SearchExpression(name){};
        virtual ~SearchString(){};
        virtual bool Matches(std::string s);
 };


class SearchRegex : public SearchExpression
{
    public:
        SearchRegex(std::string name);
        virtual ~SearchRegex(){};
        virtual bool Matches(std::string s);
    protected:
        std::regex* RegEx;
};


#endif /* HAND_SEARCHSTRING_H */
