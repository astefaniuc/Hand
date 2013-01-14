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

#include "view/layer/listlayer.h"
#include "graph/method.h"
#include "graph/data.h"


using namespace std;


ListLayer::ListLayer(string name) : Layer(name)
{
    type(LIST);
    BufferType = COLLECTOR;
    get(EXECUTE)->add(new Method<ListLayer>("SetCommand", this, &ListLayer::execute));
}


ListLayer::~ListLayer()
{
}


void ListLayer::SetContent(Vertex* data)
{
    // Connect list and layer
    Layer::SetContent(data);

    uint nr_of_childs = data->size();
    nr_of_childs--;
    Data<uint>* max_c = dynamic_cast<Data<uint>*>(get(LAYOUT, ANY)->get("MaxSize"));
    if(max_c)
    {
        uint max_size = max_c->get();
        if(max_size < nr_of_childs)
            nr_of_childs = max_size;
    }

    Vertex* child;
    uint i = 0;
    while((child=data->get(++i)) != NULL)
        // Create the sub-objects
        Insert(child, ELEMENT);
}


bool ListLayer::SetCommand(Vertex* cmd)
{
    Vertex* children = get(LINK, CHILDREN);
    if(!children)
        return false;

    Layer* child;
    uint i = 0;
    while((child=dynamic_cast<Layer*>(children->get(++i))) != NULL)
        child->Layer::SetCommand(cmd->get(i));
    return true;
}


bool ListLayer::execute(Vertex* param)
{
    SetCommand(Vertex::get(LAYERMANAGER)->get()->get(ANY, COMMANDS));
    return true;
}
