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


using namespace std;


StateNode::StateNode(uint size, Vertex* peers) : List(STATENODE)
{
    type(STATENODE);
    // Init the related Nodes container with "empty" objects
    for(uint i=1; i<=size; i++)
        add(new List(CHILD));

    Vertex::attach(peers);
    // Adds itself to the peers list
    peers->add(this);
}


Vertex* StateNode::get(string name)
{
    Vertex* ret = List::get(ANY, name);
    if(ret || (name!=VIEW))
        return ret;

    ret = List::get(VIEW);
    Vertex* active_keys;
    // Peers
    Vertex* keylist = Vertex::get(COMMANDS)->Vertex::get(DEVICE, ANY)->get(VIEW)->get(KEYLIST);
    uint i = 0;
    while((active_keys=get(++i)) != NULL)
        if(active_keys->name() == PARENT)
             ret->attach(keylist->get(i));

    return ret;
}


StateNode* StateNode::GetParent(uint k_nr)
{
    Vertex* ret = get(k_nr);
    if(ret && (ret->name() == PARENT))
        return dynamic_cast<StateNode*>(ret->get());

    return NULL;
}


StateNode* StateNode::GetChild(uint k_nr)
{
    Vertex* ret = get(k_nr);
    if(ret && (ret->name() == CHILD))
        return dynamic_cast<StateNode*>(ret->get());

    return NULL;
}
