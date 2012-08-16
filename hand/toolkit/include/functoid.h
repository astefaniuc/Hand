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

#ifndef HAND_FUNCTOID_H
#define HAND_FUNCTOID_H

#include "defines.h"

#include <string>
#include <sstream>
#include <typeinfo>
#include <vector>


using namespace std;


// Arbitrary search depth (should be big enough to find everything
// and small enough to return fast from circles)
#define MAX_SEARCH_DEPTH 1024


class FunctoidSearch;
class SearchExpression;

class Functoid : public vector<Functoid*>
{
    public:
        Functoid(string name);
        virtual ~Functoid();
        // Delete all children
        virtual void CleanUp();

        // Check first if the string "name" contains a scheme
        virtual Functoid* Find(string name, int max_depth = MAX_SEARCH_DEPTH);
        // Iterative deepening depth-first search recursive calls
        virtual Functoid* _Find(string name, int depth);
        virtual Functoid* Find(SearchExpression* expression);
        virtual bool IsOpen(FunctoidSearch* search);

        // Searches only one level down
        virtual bool Set(Functoid* child);
        // Get the child by name
        virtual Functoid* Get(string child);
        // Get the child by position, 1-based
        virtual Functoid* Get(uint child);

        virtual bool Add(Functoid* child);
        virtual bool Delete(Functoid* child);

        virtual bool Attach(Functoid* item);
        virtual bool Detach(Functoid* item);

        virtual void SetType(string type);
        virtual string GetType();
        virtual bool IsType(string type);
        virtual bool IsType(SearchExpression* type);
//        virtual string GetAsString() = 0;
        virtual bool Execute(Functoid* func_param);

        virtual bool SetOwner(Functoid* ignore);
        virtual Functoid* GetOwner();
        // Returns true if it can be deleted
        virtual bool IsOwner(Functoid* caller);

//        virtual bool NotifyChanged();
        // The following five methods operate all on the same private "Name" member
        // Set "name" with or without uri scheme
        virtual void SetName(string name);
        // Get the name without uri scheme
        virtual string& GetName();
        // Get the name with uri scheme
        virtual string GetUriString();

    protected:
        string Name;
};

typedef vector<Functoid*>::iterator FunctoidIterator;


#define FUNCTOIDDATA "FUNCTOIDDATA"
// TODO: This string varies on different platforms
#define TYPE_DATA_STRING "Ss"

template <typename I>
class Data : public Functoid
{
    public:
        Data(string name, I val) : Functoid(name)
        {
            Value = val;
            SetType(string(typeid(val).name()));
        };

        bool Set(I val)
        {
            Value = val;
            return true;
        };
        I Get()
        {
            return Value;
        };
        string GetAsString()
        {
            ostringstream s;
            s << Value;
            return s.str();
        };
   protected:
        I Value;
};

typedef Data<string> Note;


template <class I>
class Callback : public Functoid
{
    typedef bool (I::*TFunction)(Functoid*);

    public:
        Callback(string name, I* obj, TFunction func) : Functoid(name)
        {
            Object = obj;
            Function = func;
            SetType(BUTTON_ITEM);
        };
        // Clean-up object
        void Clean(void)
        {
            delete(Object);
        };

        // Execute the Callback
        bool Execute(Functoid* param)
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

        string GetAsString()
        {
            // TODO: what kind of a string to create: for the GUI? for persistent storage? for both?
            return "";
        };
    private:
        TFunction Function;
        I* Object;
};


#define TYPE_DESCRIPTOR "Link"

class Link : public Functoid
{
    public:
        Link(string name, string type, uint size=1);
        virtual ~Link();

        Functoid* Find(string name, int max_depth);
        Functoid* _Find(string name, int depth);
        virtual bool Add(Functoid* val);
        virtual bool Set(Functoid* val);
        Functoid* Get(uint i);
        virtual Functoid* Get();
        string GetAsString();
        bool Execute(Functoid* vs);
        bool IsList();
        void MakeMultiLink(bool);
        bool IsMultiLink();

   protected:
        Functoid* Value;
        bool IsMulti;
};

#endif /* HAND_FUNCTOID_H */
