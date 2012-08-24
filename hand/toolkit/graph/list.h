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

#ifndef HAND_LIST_H
#define HAND_LIST_H

#include "graph/vertex.h"


#define LIST "List"

class Relation;
class Factory;

class List : public Vertex
{
    public:
        List(std::string name);
        virtual ~List();

        // Get the child by name
        virtual Vertex* Get(std::string child);
        virtual Vertex* Get(std::string type, std::string name);
        // Get the child by position, 1-based; Element '0' stores
        // hidden system information e.g. type and layout.
        virtual Vertex* Get(uint child);

        virtual void SetType(std::string type);
        virtual std::string GetType();
        virtual bool IsType(std::string type);
        virtual bool IsType(SearchExpression* type);

        // Set object owner (for memory management)
        virtual void SetOwner(Vertex* owner);
        virtual bool HasOwner(Vertex* caller);

        // Remove all public children
        virtual void Reset();

        // ???
//        virtual Factory* GetFactory();
};


// TODO: combine Link and Relation? Or even get rid of both?
#define DESCRIPTOR "Link"

class Link : public List
{
    public:
        Link(std::string name, std::string type, bool is_multi_link=false);
        virtual ~Link(){};

        virtual bool Add(Vertex* val);
        virtual bool Set(Vertex* val);
        bool Execute(Vertex* vs);
        void MakeMultiLink(bool);
        bool IsMultiLink();

   protected:
        bool IsMulti;
};


#define RELATION "RELATION"

class Relation : public List
{
    public:
        Relation(std::string name);
        virtual ~Relation(){};
        // Inserts a sub-item to the current functoid without changing
        // the ownership of 'item'; it replaces *all* objects referenced
        // by the Relation (only public elements)
        virtual bool Set(Vertex* sub);
        // Helper method for the search engine
        bool IsOpen(VertexSearch* search);
};

#endif // HAND_LIST_H
