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

#include "input/statetree.h"


using namespace std;


StateTree::StateTree(uint size)
{
    // Create the root/null key
    RootKey = new Node(size, 0, new List(PEERS));
    // Build the key tree on top
    for(uint i=1; i<=size; i++)
        // Build the key tree bellow
        AddNodes(1, i);
}


void StateTree::AddNodes(uint level, uint key_nr)
{
    Vertex* tmp_node;
    Vertex* new_node;

    // Previous level nodes
    Vertex* p_nodes = GetPeers(level-1);
    // Next level nodes (the active one)
    Vertex* c_nodes = GetPeers(level);

    uint i = 0;
    Vertex* parent_node;
    // Create a new node for each one existing one level down
    while((parent_node=p_nodes->get(++i)) != NULL)
    {
        // Ignore nodes inserted for the current key
        if(parent_node->get(key_nr)->get() != NULL)
            continue;

        // Appends also the new node to c_nodes
        new_node = new Node(RootKey->size(), level, c_nodes);
        // Connect new and old node at key number position
        ConnectNodes(parent_node, new_node, key_nr);
        // The new node inherits from the old one the position of the parents
        for(uint j=1; j<=key_nr; j++)
        {
            if(parent_node->get(j)->name() == PARENT)
            {
                // Returns only new nodes of the previous level
                tmp_node = GetParentNode(level-1, j);
                ConnectNodes(tmp_node, new_node, j);
            }
        }
    }

    if(key_nr >= level)
        // Next level
        AddNodes(level+1, key_nr);
}


Vertex* StateTree::GetParentNode(uint level, uint pos)
{
    Vertex* level_vector = GetPeers(level);
    Vertex* node;
    uint i = 0;
    while((node=level_vector->get(++i)) != NULL)
        if(node->get(pos)->get() == NULL)
            return node;

    return NULL;
}


Vertex* StateTree::GetPeers(uint level)
{
    if(level > RootKey->size())
        return NULL;

    // Search from bottom up
    Node* tmp_node = RootKey;
    for(uint i=1; i<=level; i++)
    {
        if(tmp_node->get(i)->get() == NULL)
            // Nothing yet on this level - create level container once
            return new List(PEERS);
        tmp_node = tmp_node->GetChild(i);
    }
    return tmp_node->Vertex::get(PEERS);
}


void StateTree::ConnectNodes(Vertex* p, Vertex* c, uint i)
{
    p->get(i)->attach(c);

    Vertex* v = c->get(i);
    v->name(PARENT);
    v->attach(p);
}


Node* StateTree::GetEntryPoint()
{
    return RootKey;
}
