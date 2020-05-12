#ifndef HAND_INPUT_STATENODE_H
#define HAND_INPUT_STATENODE_H

#include "graph/collection.h"


class StateNode : public Collection
{
public:
    StateNode(unsigned size, Vertex* peers);

    // Implements set(METHOD)
    bool set(Vertex* sub);
    using Collection::get;
    // Implements get(VIEW)
    Vertex* get(std::string name);

    // Returns the sub-node at given position if it's a parent
    StateNode* GetParent(unsigned pos);
    // Returns the sub-node at given position if it's a child
    StateNode* GetChild(unsigned pos);
};


#endif // HAND_INPUT_STATENODE_H
