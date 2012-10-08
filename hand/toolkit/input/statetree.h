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

#ifndef INPUT_STATETREE_H
#define INPUT_STATETREE_H

#include "input/node.h"


class StateTree
{
    public:
        StateTree(int);
        ~StateTree();
        Node* GetEntryPoint();
    private:
        void AddNodes(int, int);
        void ConnectNodes(Node*, Node*, int);
        Node* GetParentNode(int, int, int);
        std::vector<Node*>* GetCreateLevelVector(int);
        Node* RootKey;
};


#endif /* INPUT_STATETREE_H */
