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

#include "input/node.h"
#include "graph/vertex.h"


using namespace std;


Node::Node(int size, int level, Vertex* peers) : List("Node")
{
    // Init the related Nodes container with "empty" objects
    for(int i=1; i<=size; i++)
        add(new List(CHILD));

    Vertex::attach(peers);
    // Adds itself to the peers list
    peers->add(this);
}


Node* Node::GetParent(uint k_nr)
{
    Vertex* ret = get(k_nr);
    if(ret && (ret->name() == PARENT))
        return dynamic_cast<Node*>(ret->get());

    return NULL;
}


Node* Node::GetChild(uint k_nr)
{
    Vertex* ret = get(k_nr);
    if(ret && (ret->name() == CHILD))
        return dynamic_cast<Node*>(ret->get());

    return NULL;
}
