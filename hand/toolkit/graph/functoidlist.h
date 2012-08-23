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

#ifndef HAND_FUNCTOIDLIST_H
#define HAND_FUNCTOIDLIST_H

#include "graph/functoid.h"


#define FUNCTOIDLIST "FunctoidList"

class Relation;
class Factory;

class FunctoidList : public Functoid
{
    public:
        FunctoidList(std::string name);
        virtual ~FunctoidList();

        // Get the child by name
        virtual Functoid* Get(std::string child);
        virtual Functoid* Get(std::string type, std::string name);
        // Get the child by position, 1-based; Element '0' stores
        // hidden system information e.g. type and layout.
        virtual Functoid* Get(uint child);

        virtual void SetType(std::string type);
        virtual std::string GetType();
        virtual bool IsType(std::string type);
        virtual bool IsType(SearchExpression* type);

        // Set object owner (for memory management)
        virtual void SetOwner(Functoid* owner);
        virtual bool HasOwner(Functoid* caller);

        // Remove all public children
        virtual void CleanUp();

        // ???
//        virtual Factory* GetFactory();
};


// TODO: combine Link and Relation? Or even get rid of both?
#define DESCRIPTOR "Link"

class Link : public FunctoidList
{
    public:
        Link(std::string name, std::string type, bool is_multi_link=false);
        virtual ~Link(){};

        virtual bool Add(Functoid* val);
        virtual bool Set(Functoid* val);
        bool Execute(Functoid* vs);
        void MakeMultiLink(bool);
        bool IsMultiLink();

   protected:
        bool IsMulti;
};


#define RELATION "RELATION"

class Relation : public FunctoidList
{
    public:
        Relation(std::string name);
        virtual ~Relation(){};
        // Inserts a sub-item to the current functoid without changing
        // the ownership of 'item'; it replaces *all* objects referenced
        // by the Relation (only public elements)
        virtual bool Set(Functoid* sub);
        // Helper method for the search engine
        bool IsOpen(FunctoidSearch* search);
};

#endif // HAND_FUNCTOIDLIST_H
