#ifndef HAND_GRAPH_LIST_H
#define HAND_GRAPH_LIST_H

#include "graph/vertex.h"


// Keeps a list of "public" elements in a clean(*) child vertex
// under PUBLIC.
//
// (*) no type or runtime info
//
class List : public Vertex
{
public:
    List(const std::string& name);

    // Overloads of the public interface of the Vertex class
    // to operate on the list elements
    bool add(Vertex* child) override;
    bool set(Vertex* sub) override;
    bool attach(Vertex* sub) override;

    Vertex* get(const std::string& child) override;
    Vertex* get(const std::string& type, const std::string& name) override;
    Vertex* get(uint child) override;

    uint size() override;
    // Removes all entries from the PUBLIC sub-vertex
    void reset() override;
    bool detach(Vertex* item) override;
    bool remove(Vertex* child) override;

protected:
    Vertex* _get() override;
};

#endif // HAND_GRAPH_LIST_H
