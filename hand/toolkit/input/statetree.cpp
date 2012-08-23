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


StateTree::StateTree(int size)
{
    // Create the root/null key
    vector<Node*>* lvn = new vector<Node*>;
    RootKey = new Node(size, 0, lvn);
    // Build the key tree on top
    for(int i=0; i<size; i++)
        // Build the key tree bellow
        AddNodes(1, i);
}


StateTree::~StateTree()
{
    // Deletes the whole tree - also thinkable: delete only this tree factory
    // once the tree is build
    if(RootKey == NULL)
        return;

    vector<Node*>* nodes;
    vector<Node*>::iterator c_node;
    vector<Node*>::iterator end;
    int size = RootKey->GetSize();
    // Delete tree level by level from top
    for(int i=size; i>=0; i--)
    {
        nodes = GetCreateLevelVector(i);
        end = nodes->end();
        // Delete every node of the level
        for(c_node=nodes->begin(); c_node!=end; c_node++)
            delete (*c_node);
    }
}


void StateTree::AddNodes(int level, int key_nr)
{
    Node* tmp_node;
    Node* new_node;

    // Prevoius level nodes
    vector<Node*>* p_nodes = GetCreateLevelVector(level-1);
    vector<Node*>::iterator parent_node;
    vector<Node*>::iterator end = p_nodes->end();
    // Next level nodes (the active one)
    vector<Node*>* c_nodes = GetCreateLevelVector(level);

    // Create a new node for each one existing one level down
    for(parent_node=p_nodes->begin();
        parent_node!=end;
        parent_node++)
    {
        // Ignore nodes inserted for the current key
        if((*parent_node)->GetNode(key_nr) != NULL)
            continue;

        // Appends also the new node to c_nodes
        new_node = new Node(RootKey->GetSize(), level, c_nodes);
        // Connect new and old node at key number position
        ConnectNodes((*parent_node), new_node, key_nr);
        // The new node inherits from the old one the position of the parents
        for(int i=0; i<key_nr; i++)
        {
            if((*parent_node)->GetParent(i) != NULL)
            {
                // Returns only new nodes of the previous level
                tmp_node = GetParentNode(level-1, i, key_nr);
                ConnectNodes(tmp_node, new_node, i);
            }
        }
    }

    if(key_nr >= level)
        // Next level
        AddNodes(level+1, key_nr);
}


Node* StateTree::GetParentNode(int level, int pos, int key_nr)
{
    // TODO:if to be called from the outside: check parameters and return NULL
    // if invalid
    vector<Node*>* level_vector = GetCreateLevelVector(level);
    vector<Node*>::iterator node;
    vector<Node*>::iterator end = level_vector->end();
    for(node=level_vector->begin(); node!=end; node++)
        if((*node)->GetNode(pos) == NULL)
            return (*node);

    return NULL;
}


vector<Node*>* StateTree::GetCreateLevelVector(int level)
{
    if(level > RootKey->GetSize())
        return NULL;

    // Search from bottom up
    Node* tmp_node = RootKey;
    for(int i=0; i<level; i++)
    {
        if(tmp_node->GetNode(i) == NULL)
            // Nothing yet on this level - create level container once
            return new vector<Node*>;
        tmp_node = tmp_node->GetChild(i);
    }
    return tmp_node->PeerNodes;
}


void StateTree::ConnectNodes(Node* p, Node* c, int i)
{
    p->SetNode(c, i, false);
    c->SetNode(p, i, true);
}


Node* StateTree::GetEntryPoint()
{
    return RootKey;
}
