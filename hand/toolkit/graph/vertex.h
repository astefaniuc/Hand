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

#include "include/defines.h"

#include <string>
#include <sstream>


class BaseList;
class Search;
class RegularExpression;

class Vertex
{
    public:
        Vertex(std::string name);
        virtual ~Vertex();

        // Inserts a sub-item to the current vertex and makes the current
        // vertex the owner of it if it is unowned; allows multiple objects
        // with the same name
        virtual bool Add(Vertex* child);
        // Inserts a sub-item to the current vertex without changing
        // the ownership of 'item'; it replaces any object with same name
        virtual bool Set(Vertex* sub);
        // Appends a sub-item to the current vertex without changing
        // the ownership of 'item'; allows multiple objects with same name
        virtual bool Attach(Vertex* sub);

        // Return the default child/value which is NULL for a plain vertex
        Vertex* Get();
        // Get the first sub-item by name.
        // If the item doesn't exist it returns a new Relation vertex;
        // use Get(ANY, name) to test if a vertex exists
        virtual Vertex* Get(std::string name);
        // Get the first sub-item by name and type; if the item is not found
        // it returns NULL; the parameters can be set to ANY e.g.
        // Get(ANY, ANY) returns the first stored item
        virtual Vertex* Get(std::string name, std::string type);
        // Get the child by position, 1-based
        virtual Vertex* Get(uint item);

        // Get the number of elements
        virtual uint Size();

        // Simple iterative deepening depth-first search.
        // Omits OWNER from the search but might fail with any other graph
        // cycles, use the Search engine in case of doubt.
        virtual Vertex* Find(std::string name, int max_depth = 2);
        // Flat search with support for (boost-typed) regular expressions
        // TODO: make it iterative or rename to it "Get()"?
        virtual Vertex* Find(RegularExpression* expression);

        // Set Vertex name
        void Name(std::string name);
        // Get the Vertex name
        std::string& Name();

        // Set the Vertex type/tag
        void Type(std::string type);
        // Returns the last set type
        std::string Type();
        bool Is(std::string type);
        bool Is(RegularExpression* type);

        // Removes all sub-objects
        virtual void Reset();
        // Removes child from Vertex. If Vertex is the owner of child it also
        // resets the ownership
        virtual bool Detach(Vertex* child);
        // Removes the reference to 'item' and deletes the object if it
        // is the owner
        virtual bool Delete(Vertex* child);

        // Returns the string to be visualized, here: the name
        virtual std::string GetAsString();

        // Interface to methods defined in a derived class
        virtual bool Execute(Vertex* func_param);

        // Helper method for the search engine
        virtual bool IsOpen(Search* search);

        // TODO:
//        virtual bool NotifyChanged();

    protected:
        // Helper method allowing to virtualize the return value of Get(void);
        // overload this if you want to return a different default value
        virtual Vertex* _Get();
        virtual Vertex* _Find(std::string name, int depth);

    private:
        // Set object owner (for memory management).
        void Owner(Vertex* owner);
        // Returns the pointer to the owner vertex
        Vertex* Owner();

        std::string _Name;
        BaseList* _Body;
        BaseList* _References;
        Vertex* _Type;
};

#endif /* GRAPH_VERTEX_H */
