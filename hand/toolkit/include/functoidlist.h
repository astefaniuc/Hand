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

#include "functoid.h"


using namespace std;

#define TYPE_FUNCTOIDLIST "FunctoidList"
// Reserved tags/names for Functoid elements
#define TAG_RELATION_PARENT "TAG_RELATION_PARENT"
#define TAG_RELATION_CHILD "TAG_RELATION_CHILD"
#define TAG_TYPE "TAG_TYPE"
#define TAG_DESCRIPTION "TAG_DESCRIPTION"

class Relation;
class Factory;

class FunctoidList : public Functoid
{
    public:
        FunctoidList(string name);
        virtual ~FunctoidList();

        virtual void SetType(string type);
        // Set one directional relation to given functoid
        using Functoid::Add;
        virtual bool Add(string relation_name, Functoid* functoid);
        using Functoid::Set;
        virtual bool Set(string relation_name, Functoid* functoid);

        // Get the child by name
        virtual Functoid* Get(string child);
        // Get the child by position, 0-based; overloads the FunctoidList
        // method which is 1-based. Element '0' stores hidden system information
        // e.g. type and layout. "Public" elements can be iterated through:
        /*
        uint i = 0;
        while((child=list->Get(++i)) != NULL)
            ...
        */
        virtual Functoid* Get(uint child);
        virtual Functoid* Get(string relation_name, string element);

        virtual bool IsOwner(Functoid* caller);
        // Delete all sub-functoids it owns
        virtual void CleanUp(void);
        // Remove first found reference to child from direct childs & direct relations
        virtual bool Detach(Functoid* child);
        virtual Factory* GetFactory();
};


// TODO: combine Link and Relation?
#define TYPE_DESCRIPTOR "Link"

class Link : public FunctoidList
{
    public:
        Link(string name, string type, bool is_multi_link=false);
        virtual ~Link(){};

        virtual bool Add(Functoid* val);
        virtual bool Set(Functoid* val);
        bool Execute(Functoid* vs);
        void MakeMultiLink(bool);
        bool IsMultiLink();

   protected:
        bool IsMulti;
};


#define FUNCTOIDRELATION "FUNCTOIDRELATION"

class Relation : public FunctoidList
{
    public:
        Relation(string name);
        virtual ~Relation(){};
        bool IsOpen(FunctoidSearch* search);
};

#endif // HAND_FUNCTOIDLIST_H
