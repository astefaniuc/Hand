#ifndef HAND_INPUT_STATENODE_H
#define HAND_INPUT_STATENODE_H

#include <vector>


class HmiItem;

class StateNode
{
public:
    typedef std::vector<StateNode*> PeersList;

    StateNode(unsigned size, PeersList* peers);

    // The action to be executed
    void SetAction(HmiItem* action) { m_Action = action; }
    HmiItem* GetAction() { return m_Action; }
    // Shortcut.
    void ClearAction() { m_Action = nullptr; }

    void SetParent(StateNode* parent, unsigned pos);
    // Returns the sub-node at given position if it's a parent
    StateNode* GetParent(unsigned pos);
    void SetChild(StateNode* child, unsigned pos);
    // Returns the sub-node at given position if it's a child
    StateNode* GetChild(unsigned pos);

    PeersList* GetPeersList() { return m_Peers; }

private:
    struct Link
    {
        StateNode* Item = nullptr;
        bool IsParent = false;
    };
    std::vector<Link> m_Links;
    PeersList* m_Peers;

    HmiItem* m_Action = nullptr;
};


#endif // HAND_INPUT_STATENODE_H
