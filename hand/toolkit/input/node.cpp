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
#include "view/layer/layer.h"


using namespace std;


Node::Node(int size, int level, vector<Node*>* peers_vector)
{
    NodeVector* kv;
    // Init the related Nodes container with "empty" objects
    for(int i=0; i<=size; i++)
    {
        kv = new NodeVector;
        kv->Associate = NULL;
        kv->Parent = false;
        TreeNodes.push_back(kv);
    }
    Level = level;
    PeerNodes = peers_vector;
    // Adds itself to the peers list
    PeerNodes->push_back(this);
    AssocLayer = NULL;
}


Node::~Node()
{
    vector<NodeVector*>::iterator kv;
    for(kv = TreeNodes.begin(); kv!=TreeNodes.end(); kv++)
    {
        delete (*kv);
    }
}


Node* Node::GetNode(int k_nr)
{
    vector<NodeVector*>::iterator kv = TreeNodes.begin() + k_nr;
    return (*kv)->Associate;
}


void Node::SetNode(Node* k_ptr, int k_nr, bool rel)
{
    vector<NodeVector*>::iterator kv = TreeNodes.begin() + k_nr;
    (*kv)->Associate = k_ptr;
    (*kv)->Parent = rel;
}


Node* Node::GetParent(int k_nr)
{
    vector<NodeVector*>::iterator kv = TreeNodes.begin() + k_nr;
    if((*kv)->Parent)
    {
        return (*kv)->Associate;
    }
    return NULL;
}


Node* Node::GetChild(int k_nr)
{
    vector<NodeVector*>::iterator kv = TreeNodes.begin() + k_nr;
    if(!(*kv)->Parent)
    {
        return (*kv)->Associate;
    }
    return NULL;
}


int Node::GetSize()
{
    return TreeNodes.size();
}


bool Node::SetLayer(Layer* l)
{
    // Protect submitted cmds but allow resetting StateMachine
    if((l != NULL) && (AssocLayer != NULL))
    {
        return false;
    }
    AssocLayer = l;
    return true;
}

Layer* Node::GetLayer()
{
    return AssocLayer;
}


vector<Node*>* Node::GetPeerNodes()
{
    return PeerNodes;
}
