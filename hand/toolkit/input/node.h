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

#ifndef INPUT_NODE_H
#define INPUT_NODE_H

#include "graph/list.h"


class Node : public List
{
    public:
        Node(int size, int level, Vertex* peers);
        ~Node(){};
        // Returns the sub-node at given position if it's a parent
        Node* GetParent(uint pos);
        // Returns the sub-node at given position if it's a child
        Node* GetChild(uint pos);
};


#endif /* INPUT_NODE_H */
