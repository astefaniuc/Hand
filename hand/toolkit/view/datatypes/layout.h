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

#ifndef VIEW_DATATYPES_LAYOUT_H
#define VIEW_DATATYPES_LAYOUT_H

#include "graph/list.h"


class Layout : public List
{
    public:
        Layout(std::string name, std::string type);
        virtual ~Layout(){};

        bool add(Vertex* child);
        using List::get;
        virtual Vertex* get(std::string type, std::string name);
        bool execute(Vertex* surface);
        void reset();
};

#endif /* VIEW_DATATYPES_LAYOUT_H */
