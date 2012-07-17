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

#ifndef HAND_FUNCTOIDNODE_H
#define HAND_FUNCTOIDNODE_H

#include "functoidlist.h"


using namespace std;


#define TYPE_FUNCTOIDNODE "FunctoidNode"
// Reserved tags/names for Functoid elements
#define TAG_RELATION_PARENT "TAG_RELATION_PARENT"
#define TAG_RELATION_CHILD "TAG_RELATION_CHILD"
#define TAG_TYPE "TAG_TYPE"
#define TAG_DESCRIPTION "TAG_DESCRIPTION"

class Relation;
class Factory;

class FunctoidNode : public FunctoidList
{
    public:
        FunctoidNode(string name);
        virtual ~FunctoidNode();

        virtual void SetType(string type);
        virtual bool IsType(string type);
        virtual bool IsType(SearchExpression* search);
        // Set one directional relation to given functoid
        using FunctoidList::Add;
        virtual bool Add(string relation_name, Functoid* functoid);
        using FunctoidList::Set;
        virtual bool Set(string relation_name, Functoid* functoid);
        using FunctoidList::Get;
        // Get the child by position, 0-based; overloads the FunctoidList
        // method which is 1-based. Element '0' stores type information.
        virtual Functoid* Get(uint child);
        virtual Functoid* Get(string relation_name, string element);

        virtual bool IsOwner(Functoid* caller);
        // Delete all sub-functoids it owns
        virtual void CleanUp(void);
        // Remove first found reference to child from direct childs & direct relations
        virtual bool Detach(Functoid* child);
        virtual Factory* GetFactory();
};


#define FUNCTOIDRELATION "FUNCTOIDRELATION"

class Relation : public FunctoidList
{
    public:
        Relation(string name);
        virtual ~Relation(){};
        bool IsOpen(FunctoidSearch* search);
};


#define FUNCTOIDTYPE "FUNCTOIDTYPE"

class FunctoidType : public Functoid
{
    public:
        FunctoidType(string name);
        virtual ~FunctoidType(){};
        virtual string GetAsString();
};

#endif // HAND_FUNCTOIDNODE_H
