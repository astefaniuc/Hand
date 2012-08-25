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

#ifndef HAND_VERTEX_H
#define HAND_VERTEX_H

#include "include/defines.h"

#include <string>
#include <sstream>
#include <vector>


// Arbitrary search depth (should be big enough to find everything
// and small enough to return fast from circles)
#define VERTEX "Vertex"
#define MAX_SEARCH_DEPTH 1024


class Search;
class RegularExpression;

class Vertex : public std::vector<Vertex*>
{
    public:
        Vertex(std::string name);
        virtual ~Vertex();

        // Inserts a sub-item to the current functoid and makes the current
        // functoid the owner of it if it is unowned; allows multiple objects
        // with the same name
        virtual bool Add(Vertex* child);
        // Inserts a sub-item to the current functoid without changing
        // the ownership of 'item'; it replaces any object with same name
        virtual bool Set(Vertex* sub);
        // Appends a sub-item to the current functoid without changing
        // the ownership of 'item'; allows multiple objects with same name
        virtual bool Attach(Vertex* sub);

        // Removes the reference to 'item' but keeps item alive
        virtual bool Detach(Vertex* item);
        // Removes the reference to 'item' and deletes the object if it
        // is the owner
        virtual bool Delete(Vertex* child);

        // Get the first sub-item by name.
        // If the item doesn't exist it returns a new Relation functoid;
        // use Get(ANY, name) to test if a functoid exists
        virtual Vertex* Get(std::string name);
        // Get the first sub-item by name and type; if the item is not found
        // it returns NULL; the parameters can be set to ANY e.g.
        // Get(ANY, ANY) returns the first stored item
        virtual Vertex* Get(std::string name, std::string type);
        // Interface to method defined in a derived class; returns always
        // NULL in this base class
        virtual Vertex* Get(uint item);

        // Simple iterative deepening depth-first search.
        // Omits OWNER from the search but might fail with any other graph
        // cycles, use the Search engine in case of doubt.
        virtual Vertex* Find(std::string name, int max_depth = 2);
        // Flat search with support for (boost-typed) regular expressions
        // TODO: make it iterative or rename to it "Get()"?
        virtual Vertex* Find(RegularExpression* expression);

        // Set "name" with or without uri scheme
        virtual void SetName(std::string name);
        // Get the name without uri scheme
        virtual std::string& GetName();
        // Get the name with uri scheme
        virtual std::string GetUriString();

        virtual void SetType(std::string type);
        // Returns the last set type
        virtual std::string GetType();
        virtual bool IsType(std::string type);
        virtual bool IsType(RegularExpression* type);

        // Set object owner (for memory management).
        // Owner is stored under OWNER
        virtual void SetOwner(Vertex* owner);
        // Returns true if 'caller' is the owner or if no owner is registered
        virtual bool HasOwner(Vertex* caller);

        // Detach all objects not owned
        virtual void Reset();

        // Interface to methods defined in a derived class
        virtual bool Execute(Vertex* func_param);

        // Helper method for the search engine
        virtual bool IsOpen(Search* search);
        // TODO:
//        virtual string GetAsString() = 0;
//        virtual bool NotifyChanged();

    protected:
        virtual Vertex* _Find(std::string name, int depth);

        std::string Name;
};

typedef std::vector<Vertex*>::iterator VertexIterator;

#endif /* HAND_VERTEX_H */
