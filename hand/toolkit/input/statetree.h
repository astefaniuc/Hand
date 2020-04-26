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
    void AddNodes(uint level, uint key_nr);
    void ConnectNodes(Vertex* parent, Vertex* child, uint pos);
    Vertex* GetParentNode(uint level, uint pos);
    Vertex* GetCommands(uint level);
    Vertex* GetNewPeersList();

    StateNode* Root = nullptr;
};


#endif // HAND_INPUT_STATEGRAPH_H
