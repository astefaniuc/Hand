#ifndef HAND_INPUT_STATENODE_H
#define HAND_INPUT_STATENODE_H

#include <vector>


class Layer;
struct Chord;

class StateNode
{
public:
    typedef std::vector<StateNode*> PeersList;

    StateNode(unsigned size, PeersList* peers);
    ~StateNode();

    void SetParent(StateNode* parent, unsigned pos);
    /// Returns the sub-node at given position if it's a parent
    StateNode* GetParent(unsigned pos);
    void SetChild(StateNode* child, unsigned pos);
    /// Returns the sub-node at given position if it's a child
    StateNode* GetChild(unsigned pos);

    StateNode* GetFirstChild() const;

    PeersList* GetPeersList() { return m_Peers; }

    Chord* GetChord();

private:
    struct Link
    {
        StateNode* Item = nullptr;
        bool IsParent = false;
    };
    std::vector<Link> m_Links;
    PeersList* m_Peers;

    Chord* m_Chord = nullptr;
};


#endif // HAND_INPUT_STATENODE_H
