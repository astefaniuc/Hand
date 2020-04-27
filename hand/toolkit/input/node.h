#ifndef HAND_INPUT_STATENODE_H
#define HAND_INPUT_STATENODE_H

#include "graph/list.h"


class StateNode : public List
{
public:
    StateNode(unsigned size, Vertex* peers);

    // Implements set(METHOD)
    bool set(Vertex* sub);
    using List::get;
    // Implements get(VIEW)
    Vertex* get(std::string name);

    // Returns the sub-node at given position if it's a parent
    StateNode* GetParent(unsigned pos);
    // Returns the sub-node at given position if it's a child
    StateNode* GetChild(unsigned pos);
};


#endif // HAND_INPUT_STATENODE_H
