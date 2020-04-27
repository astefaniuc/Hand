#ifndef HAND_INPUT_STATEGRAPH_H
#define HAND_INPUT_STATEGRAPH_H

#include "graph/vertex.h"


class StateNode;

class StateGraph : public Vertex
{
public:
    StateGraph();

    bool execute(Vertex* device) override;

private:
    void AddNodes(unsigned level, unsigned key_nr);
    void ConnectNodes(Vertex* parent, Vertex* child, unsigned pos);
    Vertex* GetParentNode(unsigned level, unsigned pos);
    Vertex* GetPeersList(unsigned level);
    Vertex* GetNewPeersList();

    StateNode* Root = nullptr;
};


#endif // HAND_INPUT_STATEGRAPH_H
