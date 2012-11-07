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

#ifndef GRAPH_VERTEX_H
#define GRAPH_VERTEX_H

#include <string>
#include <sstream>
#include "include/defines.h"


class BaseList;
class Search;
class RegularExpression;

class Vertex
{
    public:
        Vertex(std::string name);
        virtual ~Vertex();

        // Appends (if not already there) a sub-item to the current vertex
        // and makes the current vertex the owner of it
         virtual bool add(Vertex* child);
        // Inserts (or moves) a sub-item into the current vertex at first position
        // ("default" value returned by get()) without changing the ownership of 'sub'
        virtual bool set(Vertex* sub);
        // Appends (if not already there) a sub-item to the current vertex
        // without changing the ownership of 'sub'
        virtual bool attach(Vertex* sub);

        // Return the default child/value which is NULL for a plain vertex
        Vertex* get();
        // Get the first sub-item by name.
        // If the item doesn't exist it returns a new Link vertex;
        // use get(ANY, name) to test if a vertex exists
        virtual Vertex* get(std::string name);
        // Get the first sub-item by name and type; if the item is not found
        // it returns NULL; the parameters can be set to ANY e.g.
        // get(ANY, ANY) returns the first stored item
        virtual Vertex* get(std::string type, std::string name);
        // Get the child by position, 1-based
        virtual Vertex* get(uint item);

        // Get the number of elements
        virtual uint size();

        // Simple iterative deepening depth-first search.
        // Omits OWNER from the search but might fail with any other graph
        // cycles, use the Search engine in case of doubt.
        virtual Vertex* find(std::string name, int max_depth = 2);
        // Flat search with support for (boost-typed) regular expressions
        // TODO: make it iterative or rename to it "get()"?
        virtual Vertex* find(RegularExpression* expression);

        // Set Vertex name
        void name(std::string name);
        // Get the Vertex name
        std::string& name();

        // Set the Vertex type/tag
        void type(std::string type);
        // Returns the last set type
        std::string type();
        bool is(std::string type);
        bool is(RegularExpression* type);

        // Removes all sub-objects
        virtual void reset();
        // Removes child from Vertex. If Vertex is the owner of child it also
        // resets the ownership
        virtual bool detach(Vertex* child);
        // Removes the reference to 'item' and deletes the object if it
        // is the owner
        virtual bool remove(Vertex* child);

        // Returns the string to be visualized, here: the name
        virtual std::string getAsString();

        // Interface to methods defined in a derived class
        virtual bool execute(Vertex* func_param);

        // Helper method for the search engine
        virtual bool isOpen(Search* search);

    protected:
        // Helper method allowing to virtualize the return value of get(void);
        // overload this if you want to return a different default value
        virtual Vertex* _get();
        virtual Vertex* _find(std::string name, int depth);

    private:
        // Set object owner (for memory management).
        void owner(Vertex* owner);
        // Returns the pointer to the owner vertex
        Vertex* owner();

        std::string Name;
        BaseList* Body;
        BaseList* References;
        Vertex* Type;
};

#endif /* GRAPH_VERTEX_H */
