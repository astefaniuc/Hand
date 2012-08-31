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

#ifndef TOOLKIT_GRAPH_DATA_H
#define TOOLKIT_GRAPH_DATA_H

#include "graph/vertex.h"

#include <sstream>
#include <typeinfo>


template <typename I>
class Data : public Vertex
{
    public:
        Data(std::string name, I val) : Vertex(name)
        {
            Value = val;
            Type(DATA);
            Type(std::string(typeid(val).name()));
        };

        bool Set(I val)
        {
            Value = val;
            return true;
        };
        I Get()
        {
            return Value;
        };
        std::string GetAsString()
        {
            std::ostringstream s;
            s << Value;
            return s.str();
        };
   protected:
        I Value;
};

typedef Data<std::string> Note;

#endif /* TOOLKIT_GRAPH_DATA_H */
