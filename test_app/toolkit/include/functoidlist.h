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

#include <vector>


using namespace std;


#define TYPE_FUNCTOIDLIST "FunctoidList"
typedef vector<Functoid*>::iterator FunctoidIterator;

class FunctoidList : public Functoid, public vector<Functoid*>
{
    public:
        FunctoidList(string name);
        virtual ~FunctoidList(void);
        // Iterative deepening depth-first search entry functions
        virtual Functoid* Find(string name, int max_depth=MAX_SEARCH_DEPTH);
        virtual bool IsOpen(FunctoidSearch* search);
        virtual bool IsList();
        // Iterative deepening depth-first search recursive calls
        virtual Functoid* _Find(string name, int depth);
        virtual Functoid* Find(SearchExpression* expression);

        // Insert child to the list of children; if a child with the same name
        // already exist it replaces that one
        virtual bool Set(Functoid* child);
        // Get the child by name
        virtual Functoid* Get(string child);
        // Get the child by position, 1-based
        virtual Functoid* Get(uint child);
        // Append child to the list of children without checking if a child with
        // the same name already exist
        virtual bool Add(Functoid* child);
        // Remove first found reference to child (only from direct children)
        // and delete the object
        virtual bool Delete(Functoid* child);

        virtual bool Attach(Functoid* item);
        // Remove first found reference to item (only from first level)
        virtual bool Detach(Functoid* item);

        virtual string GetAsString();
        // Delete all children
        virtual void CleanUp();
};

#endif // HAND_FUNCTOIDLIST_H
