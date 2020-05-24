#ifndef HAND_INPUT_STATEGRAPH_H
#define HAND_INPUT_STATEGRAPH_H

#include "input/node.h"


class StateNode;

class StateGraph
{
public:
    StateNode* Create(unsigned numberOfKeys);

private:
    void AddNodes(unsigned level, unsigned keyNumber);
    void ConnectNodes(StateNode* parent, StateNode* child, unsigned pos);
    StateNode* GetParentNode(unsigned level, unsigned pos);
    StateNode::PeersList* GetPeersList(unsigned level);

    StateNode* m_Root = nullptr;
    unsigned m_NumberOfKeys = 0;
};


#endif // HAND_INPUT_STATEGRAPH_H
