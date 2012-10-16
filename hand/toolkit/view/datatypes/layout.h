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

#include "include/defines.h"
#include "base/factory.h"
#include "graph/data.h"
#include "graph/link.h"


enum alignment
{
    HORIZONTAL,
    VERTICAL
};

class Rgb;
class Rect;

class Layout : public List
{
    public:
        Layout(std::string name, std::string type);
        virtual ~Layout(){};

        Vertex* GetField(std::string position);
        Vertex* AddField(std::string name);
        void AddForUpdate(Vertex* sublayout);
        bool Execute(Vertex* surface);
        void Reset();
};


template <class I>
class LayoutFactory : public Factory
{
    typedef bool (I::*Method)(Vertex*);

    public:
        LayoutFactory(std::string output_type, I* theme, Method func) :
            Factory("LayoutFactory", LAYOUT, output_type)
        {
            Producer = theme;
            Function = func;
        };
        virtual ~LayoutFactory(){};

        bool Execute(Vertex* in_out)
        {
            if((Producer->*Function)(in_out))
                return true;
            return false;
        };

    private:
        Method Function;
        I* Producer;
};


template <class I>
class PropertyFactory : public Factory
{
    typedef bool (I::*Method)(Vertex*);

    public:
    PropertyFactory(std::string output_type, I* theme, Method func) :
            Factory("PropertyFactory", LINK, output_type)
        {
            Producer = theme;
            Function = func;
        };
        virtual ~PropertyFactory(){};

        bool Execute(Vertex* in_out)
        {
            return (Producer->*Function)(in_out);
        };

    private:
        Method Function;
        I* Producer;
};

#endif /* VIEW_DATATYPES_LAYOUT_H */
