#ifndef HAND_GRAPH_VERTEX_H
#define HAND_GRAPH_VERTEX_H

#include <string>
#include <sstream>
#include "include/defines.h"


class BaseList;
class Search;
class RegularExpression;

class Vertex
{
public:
    Vertex(const std::string& name);
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

    // Return the default child/value which is nullptr for a plain vertex
    Vertex* get();
    // Get the first sub-item by name.
    // If the item doesn't exist it returns a new Link vertex;
    // use get(ANY, name) to test if a vertex exists
    virtual Vertex* get(const std::string& name);
    // Get the first sub-item by name and type; if the item is not found
    // it returns nullptr; the parameters can be set to ANY to ignore that value;
    // get(ANY, ANY) ignores the type but does search for item named ANY ("*")
    virtual Vertex* get(const std::string& type, const std::string& name);
    // Get the child by position, 1-based
    virtual Vertex* get(uint item);

    // Get the number of elements
    virtual uint size();

    // Simple iterative deepening depth-first search.
    // Omits OWNER from the search but might fail with any other graph
    // cycles, use the Search engine in case of doubt.
    virtual Vertex* find(const std::string& name, int max_depth = 2);
    // Flat search with support for (boost-typed) regular expressions
    // TODO: make it iterative or rename to it "get()"?
    virtual Vertex* find(RegularExpression* expression);

    // Set Vertex name
    void name(const std::string& name){ Name = name; }
    // Get the Vertex name
    const std::string& name() { return Name; }


    // Set the Vertex type/tag
    void type(const std::string& type);
    // Returns the last set type
    std::string type();
    bool is(const std::string& type);
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
    virtual std::string getAsString() { return Name; }

    // Interface to methods defined in a derived class
    virtual bool execute(Vertex* func_param) { return false; }

    // Helper method for the search engine
    virtual bool isOpen(Search* search);

protected:
    // Helper method allowing to virtualize the return value of get(void);
    // overload this if you want to return a different default value
    virtual Vertex* _get();
    virtual Vertex* _find(const std::string& name, int depth);

private:
    // Set object owner (for memory management).
    void setOwner(Vertex* owner);
    // Returns the pointer to the owner vertex
    Vertex* getOwner();

    std::string Name;
    BaseList* Body;
    BaseList* References;
    Vertex* Type = nullptr;
};

#endif // HAND_GRAPH_VERTEX_H
