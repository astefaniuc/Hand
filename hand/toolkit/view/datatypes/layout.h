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
#include "graph/method.h"


class Layout : public List
{
    public:
        Layout(std::string name);
        virtual ~Layout(){};

        // Implements add(sub_layout), a sub_layout is a fixed field
        bool add(Vertex* child);

        using List::get;
        // Implements get(FIELD, name) used in Layer classes to poll
        // the availability of a specific field
        Vertex* get(std::string type, std::string name);

        bool execute(Vertex* surface);
        void reset();
};


class FieldsContainer : public List
{
    public:
        FieldsContainer(Vertex* parent_layout) : List(FIELDS)
        {
            Vertex::get(PARENT)->set(parent_layout);
        };
        ~FieldsContainer(){};

        using List::get;
        // Iterates through sub-layouts (filled fields)
        Vertex* get(uint item);
        // Adds a Vertex::Link to the parent layout to new fields
        Vertex* get(std::string name);

        // Returns the number of sub-layouts (filled fields)
        uint size();
};


template <class I>
class LayoutFactory : public Method<I>
{
    typedef bool (I::*TFunction)(Vertex*);

    public:
        LayoutFactory(std::string name, I* obj, TFunction func) : Method<I>(name, obj, func)
        {
            Vertex::type(FACTORY);
        };
        virtual ~LayoutFactory(){};

        Vertex* _get()
        {
            Vertex* ret = new Layout(Vertex::name());
            Vertex::add(ret);
            Method<I>::execute(ret);
            return ret;
        };
};

#endif /* VIEW_DATATYPES_LAYOUT_H */
