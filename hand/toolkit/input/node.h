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

#ifndef HAND_NODE_H
#define HAND_NODE_H

#include "include/defines.h"

#include <string>
#include <vector>


using namespace std;


class Layer;

class Node
{
    public:
        Node(int, int, vector<Node*>*);
        ~Node();
        void SetNode(Node*, int, bool);
        // Returns the sub-node at given position
        Node* GetNode(int);
        // Returns the sub-node at given position if it's a parent
        Node* GetParent(int);
        // Returns the sub-node at given position if it's a child
        Node* GetChild(int);
        // Returns the size of the tree node vector (nr. of keys)
        int GetSize(void);
        // Target Layer setter and getter
        bool SetLayer(Layer* cmd_target);
        Layer* GetLayer();

        vector<Node*>* GetPeerNodes(void);

    private:
        struct NodeVector
        {
            Node* Associate;
            bool Parent;
        };

        int Level;
        vector<NodeVector*> TreeNodes;
        // TEMP:
        friend class StateTree;
        vector<Node*>* PeerNodes;
        Layer* AssocLayer;
};


#endif /* HAND_NODE_H */
