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

#include "graph/vertex.h"


class StateNode;

class StateGraph : public Vertex
{
    public:
        StateGraph();
        ~StateGraph(){};

        bool execute(Vertex* device);

    private:
        void AddNodes(uint level, uint key_nr);
        void ConnectNodes(Vertex* parent, Vertex* child, uint pos);
        Vertex* GetParentNode(uint level, uint pos);
        Vertex* GetCommands(uint level);
        Vertex* GetNewPeersList();

        StateNode* Root;
};


#endif /* INPUT_STATETREE_H */
