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

#ifndef TOOLKIT_GRAPH_METHOD_H
#define TOOLKIT_GRAPH_METHOD_H

#include "graph/vertex.h"


template <class I>
class Method : public Vertex
{
    typedef bool (I::*TFunction)(Vertex*);

    public:
        Method(std::string name, I* obj, TFunction func) : Vertex(name)
        {
            Object = obj;
            Function = func;
            SetType(METHOD);
        };
        // Clean-up object
        void Clean(void)
        {
            delete(Object);
        };

        // Execute the Method
        bool Execute(Vertex* param)
        {
            if(Function)
                return (Object->*Function)(param);
            return false;
        };

        void Set(TFunction func)
        {
            Function = func;
        };
        virtual I* Get()
        {
            // Only makes sense to return the object
            return Object;
        };

        std::string GetAsString()
        {
            // TODO: what kind of a string to create: for the GUI? for persistent storage? for both?
            return "";
        };
    private:
        TFunction Function;
        I* Object;
};

#endif /* TOOLKIT_GRAPH_METHOD_H */
